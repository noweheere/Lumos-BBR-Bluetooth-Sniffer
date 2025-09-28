/*
 * Lumos BBR Bluetooth Sniffer
 * ESP32 Bluetooth sniffer for Lumos Matrix helmet communication analysis
 * 
 * This sketch captures and analyzes Bluetooth Low Energy (BLE) communications
 * from Lumos Matrix helmets to understand signal patterns for turn indicators
 * and relay commands to trailer systems.
 * 
 * Author: noweheere
 * License: MIT
 */

#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEScan.h"
#include "BLEAdvertisedDevice.h"
#include <WiFi.h>
#include <ArduinoJson.h>

// Configuration
#define SCAN_TIME 5 // seconds
#define BAUD_RATE 115200
#define LED_PIN 2
#define RELAY_PIN 23
#define LUMOS_SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"

// Global variables
BLEScan* pBLEScan;
bool deviceFound = false;
String lumosAddress = "";
unsigned long lastSignalTime = 0;
bool leftIndicator = false;
bool rightIndicator = false;
bool hazardLights = false;

// Signal pattern analysis
struct SignalPattern {
  String deviceAddress;
  String signalType;
  unsigned long timestamp;
  int rssi;
  String data;
};

std::vector<SignalPattern> signalHistory;

// BLE Advertised Device Callbacks
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    
    // Check if this is a Lumos device
    if (isLumosDevice(advertisedDevice)) {
      Serial.println("Found Lumos device!");
      Serial.printf("Address: %s\n", advertisedDevice.getAddress().toString().c_str());
      Serial.printf("Name: %s\n", advertisedDevice.getName().c_str());
      Serial.printf("RSSI: %d\n", advertisedDevice.getRSSI());
      
      // Store device info
      lumosAddress = advertisedDevice.getAddress().toString().c_str();
      deviceFound = true;
      
      // Analyze advertisement data
      analyzeAdvertisementData(advertisedDevice);
      
      // Blink LED to indicate detection
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
    }
  }
  
private:
  bool isLumosDevice(BLEAdvertisedDevice& device) {
    // Check for Lumos-specific identifiers
    String deviceName = device.getName().c_str();
    String deviceAddress = device.getAddress().toString().c_str();
    
    // Common Lumos device patterns
    if (deviceName.indexOf("Lumos") != -1 || 
        deviceName.indexOf("Matrix") != -1 ||
        deviceName.indexOf("LUMOS") != -1) {
      return true;
    }
    
    // Check for specific service UUIDs
    if (device.haveServiceUUID()) {
      if (device.isAdvertisingService(BLEUUID(LUMOS_SERVICE_UUID))) {
        return true;
      }
    }
    
    // Additional MAC address patterns (if known)
    // Lumos devices might have specific OUI patterns
    
    return false;
  }
  
  void analyzeAdvertisementData(BLEAdvertisedDevice& device) {
    SignalPattern pattern;
    pattern.deviceAddress = device.getAddress().toString().c_str();
    pattern.timestamp = millis();
    pattern.rssi = device.getRSSI();
    
    // Extract manufacturer data if available
    if (device.haveManufacturerData()) {
      std::string manufData = device.getManufacturerData();
      pattern.data = "Manufacturer: ";
      for (int i = 0; i < manufData.length(); i++) {
        pattern.data += String(manufData[i], HEX) + " ";
      }
      
      // Analyze for turn signal patterns
      analyzeTurnSignalPattern(manufData);
    }
    
    // Extract service data
    if (device.haveServiceData()) {
      std::string serviceData = device.getServiceData();
      pattern.data += "Service: ";
      for (int i = 0; i < serviceData.length(); i++) {
        pattern.data += String(serviceData[i], HEX) + " ";
      }
    }
    
    // Store pattern
    signalHistory.push_back(pattern);
    
    // Limit history size
    if (signalHistory.size() > 100) {
      signalHistory.erase(signalHistory.begin());
    }
    
    // Output analysis
    outputSignalAnalysis(pattern);
  }
  
  void analyzeTurnSignalPattern(const std::string& data) {
    // Reset indicators
    leftIndicator = false;
    rightIndicator = false;
    hazardLights = false;
    
    // Analyze byte patterns for turn signals
    // This is where you'd implement the specific protocol analysis
    // based on reverse engineering of Lumos helmet signals
    
    if (data.length() >= 4) {
      uint8_t byte1 = data[0];
      uint8_t byte2 = data[1];
      uint8_t byte3 = data[2];
      uint8_t byte4 = data[3];
      
      // Example pattern analysis (these would need to be determined through testing)
      // Left turn pattern detection
      if ((byte1 == 0x01 && byte2 == 0x00) || 
          (byte3 & 0x01) == 0x01) {
        leftIndicator = true;
        pattern.signalType = "LEFT_TURN";
        Serial.println("LEFT TURN DETECTED");
        activateRelay(1); // Left turn relay
      }
      
      // Right turn pattern detection  
      if ((byte1 == 0x02 && byte2 == 0x00) || 
          (byte3 & 0x02) == 0x02) {
        rightIndicator = true;
        pattern.signalType = "RIGHT_TURN";
        Serial.println("RIGHT TURN DETECTED");
        activateRelay(2); // Right turn relay
      }
      
      // Hazard lights pattern
      if ((byte1 == 0x03 && byte2 == 0x00) || 
          (byte3 & 0x03) == 0x03) {
        hazardLights = true;
        pattern.signalType = "HAZARD";
        Serial.println("HAZARD LIGHTS DETECTED");
        activateRelay(3); // Hazard relay
      }
      
      // Brake light pattern
      if ((byte4 & 0x80) == 0x80) {
        pattern.signalType = "BRAKE";
        Serial.println("BRAKE LIGHT DETECTED");
        activateRelay(4); // Brake relay
      }
    }
    
    lastSignalTime = millis();
  }
  
  void outputSignalAnalysis(const SignalPattern& pattern) {
    // Create JSON output for logging/debugging
    StaticJsonDocument<500> doc;
    doc["timestamp"] = pattern.timestamp;
    doc["device"] = pattern.deviceAddress;
    doc["rssi"] = pattern.rssi;
    doc["signal_type"] = pattern.signalType;
    doc["data"] = pattern.data;
    doc["left_indicator"] = leftIndicator;
    doc["right_indicator"] = rightIndicator;
    doc["hazard_lights"] = hazardLights;
    
    String output;
    serializeJson(doc, output);
    Serial.println("SIGNAL_ANALYSIS: " + output);
  }
};

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Lumos BBR Bluetooth Sniffer Starting...");
  
  // Initialize pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);
  
  // Initialize BLE
  BLEDevice::init("Lumos-Sniffer");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
  
  Serial.println("BLE Scanner initialized");
  Serial.println("Scanning for Lumos devices...");
  
  // Print configuration
  Serial.printf("Scan time: %d seconds\n", SCAN_TIME);
  Serial.printf("LED Pin: %d\n", LED_PIN);
  Serial.printf("Relay Pin: %d\n", RELAY_PIN);
  Serial.printf("Target Service UUID: %s\n", LUMOS_SERVICE_UUID);
}

void loop() {
  // Start BLE scan
  BLEScanResults foundDevices = pBLEScan->start(SCAN_TIME, false);
  
  Serial.printf("Scan completed. Found %d devices.\n", foundDevices.getCount());
  
  // Check if signal timeout (no signals for 5 seconds)
  if (millis() - lastSignalTime > 5000) {
    leftIndicator = false;
    rightIndicator = false;
    hazardLights = false;
    digitalWrite(RELAY_PIN, LOW);
  }
  
  // Status update
  printStatus();
  
  // Clear scan results
  pBLEScan->clearResults();
  
  delay(100);
}

void activateRelay(int signalType) {
  // Activate appropriate relay output
  // This could be expanded to control multiple relays for different signals
  digitalWrite(RELAY_PIN, HIGH);
  delay(500); // Signal duration
  digitalWrite(RELAY_PIN, LOW);
  
  // Send command to trailer ESP32 via Serial or WiFi
  sendTrailerCommand(signalType);
}

void sendTrailerCommand(int signalType) {
  // Prepare command for trailer ESP32
  StaticJsonDocument<200> command;
  command["type"] = "signal";
  command["signal"] = signalType;
  command["timestamp"] = millis();
  
  String cmdString;
  serializeJson(command, cmdString);
  
  // Send via Serial (if directly connected) or WiFi
  Serial.println("TRAILER_CMD: " + cmdString);
  
  // TODO: Implement WiFi/ESP-NOW communication to trailer ESP32
}

void printStatus() {
  Serial.println("=== STATUS ===");
  Serial.printf("Device found: %s\n", deviceFound ? "YES" : "NO");
  if (deviceFound) {
    Serial.printf("Lumos address: %s\n", lumosAddress.c_str());
  }
  Serial.printf("Left indicator: %s\n", leftIndicator ? "ON" : "OFF");
  Serial.printf("Right indicator: %s\n", rightIndicator ? "ON" : "OFF");
  Serial.printf("Hazard lights: %s\n", hazardLights ? "ON" : "OFF");
  Serial.printf("Signal history: %d entries\n", signalHistory.size());
  Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
  Serial.println("=============");
}

// Utility functions for signal pattern analysis
bool isValidSignalPattern(const std::string& data) {
  // Implement validation logic for Lumos signal patterns
  if (data.length() < 2) return false;
  
  // Add more sophisticated validation based on protocol analysis
  return true;
}

String bytesToHex(const std::string& data) {
  String hex = "";
  for (int i = 0; i < data.length(); i++) {
    if (data[i] < 16) hex += "0";
    hex += String(data[i], HEX);
    hex += " ";
  }
  return hex;
}

void dumpSignalHistory() {
  Serial.println("=== SIGNAL HISTORY ===");
  for (const auto& pattern : signalHistory) {
    Serial.printf("Time: %lu, Device: %s, Type: %s, RSSI: %d\n",
                  pattern.timestamp, pattern.deviceAddress.c_str(),
                  pattern.signalType.c_str(), pattern.rssi);
  }
  Serial.println("=====================");
}