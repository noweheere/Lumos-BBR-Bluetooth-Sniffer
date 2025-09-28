/*
 * Lumos BBR Bluetooth Sniffer
 * ESP32 BLE scanner for analyzing Lumos Matrix helmet communications
 * 
 * This sketch implements:
 * - BLE scanning and device discovery
 * - Signal pattern analysis for Lumos helmet communications
 * - Data logging and analysis
 * - Communication relay to trailer LED controller
 * 
 * Author: noweheere
 * License: MIT
 */

#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEScan.h"
#include "BLEAdvertisedDevice.h"
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <ArduinoJson.h>

// Configuration
#define SCAN_TIME 30
#define LED_PIN 2
#define SERIAL_BAUD 115200
#define MAX_DEVICES 50
#define SIGNAL_BUFFER_SIZE 1000

// Web server
AsyncWebServer server(80);

// BLE scan variables
BLEScan* pBLEScan;
bool scanActive = false;
unsigned long lastScanTime = 0;
unsigned long scanInterval = 5000; // 5 seconds between scans

// Device tracking
struct LumosDevice {
  String address;
  String name;
  int rssi;
  String manufacturerData;
  unsigned long lastSeen;
  int signalCount;
  bool isLumosDevice;
};

LumosDevice detectedDevices[MAX_DEVICES];
int deviceCount = 0;

// Signal pattern analysis
struct SignalPattern {
  unsigned long timestamp;
  String deviceAddress;
  int rssi;
  String data;
  String patternType;
};

SignalPattern signalBuffer[SIGNAL_BUFFER_SIZE];
int signalBufferIndex = 0;

// Known Lumos patterns (to be discovered through reverse engineering)
const String LUMOS_MANUFACTURER_ID = ""; // To be determined
const String LUMOS_SERVICE_UUID = ""; // To be determined

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    // Basic device info
    String address = advertisedDevice.getAddress().toString().c_str();
    String name = advertisedDevice.haveName() ? advertisedDevice.getName().c_str() : "Unknown";
    int rssi = advertisedDevice.getRSSI();
    
    // Check if this might be a Lumos device
    bool isLumos = false;
    String manufacturerData = "";
    String patternType = "unknown";
    
    // Analyze manufacturer data
    if (advertisedDevice.haveManufacturerData()) {
      std::string manData = advertisedDevice.getManufacturerData();
      manufacturerData = "";
      for (int i = 0; i < manData.length(); i++) {
        char hex[3];
        sprintf(hex, "%02X", (unsigned char)manData[i]);
        manufacturerData += hex;
        if (i < manData.length() - 1) manufacturerData += " ";
      }
      
      // Check for Lumos patterns
      isLumos = analyzeLumosPattern(manufacturerData, name, address);
      if (isLumos) {
        patternType = classifySignalPattern(manufacturerData, rssi);
      }
    }
    
    // Check for specific service UUIDs
    if (advertisedDevice.haveServiceUUID()) {
      std::string serviceUUID = advertisedDevice.getServiceUUID().toString();
      // Add known Lumos service UUID checks here
    }
    
    // Log interesting devices
    if (isLumos || name.indexOf("Lumos") >= 0 || rssi > -50) {
      Serial.printf("[%lu] Device: %s (%s) RSSI: %d dBm", 
                   millis(), address.c_str(), name.c_str(), rssi);
      if (isLumos) Serial.print(" [LUMOS DETECTED]");
      Serial.println();
      
      if (manufacturerData.length() > 0) {
        Serial.printf("  Manufacturer Data: %s\n", manufacturerData.c_str());
      }
      
      if (isLumos) {
        Serial.printf("  Pattern Type: %s\n", patternType.c_str());
      }
    }
    
    // Store or update device
    updateDeviceList(address, name, rssi, manufacturerData, isLumos);
    
    // Add to signal buffer for analysis
    addToSignalBuffer(address, rssi, manufacturerData, patternType);
    
    // Send signal to trailer controller if Lumos device detected
    if (isLumos) {
      relaySignalToTrailer(patternType, rssi, manufacturerData);
    }
  }
};

void setup() {
  Serial.begin(SERIAL_BAUD);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("=== Lumos BBR Bluetooth Sniffer Starting ===");
  
  // Initialize SPIFFS for web interface
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  
  // Initialize BLE
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
  
  // Setup WiFi AP for configuration
  setupWiFiAP();
  
  // Setup web server
  setupWebServer();
  
  Serial.println("Setup complete. Starting BLE scan...");
  startScan();
}

void loop() {
  // Check if scan should be restarted
  if (!scanActive && (millis() - lastScanTime > scanInterval)) {
    startScan();
  }
  
  // Process any pending web server requests
  delay(100);
  
  // Blink LED to show activity
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink > 1000) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    lastBlink = millis();
  }
  
  // Periodic signal analysis
  static unsigned long lastAnalysis = 0;
  if (millis() - lastAnalysis > 10000) { // Every 10 seconds
    analyzeSignalPatterns();
    lastAnalysis = millis();
  }
}

void startScan() {
  if (!scanActive) {
    Serial.println("Starting BLE scan...");
    pBLEScan->start(SCAN_TIME, false);
    scanActive = true;
    lastScanTime = millis();
  }
}

bool analyzeLumosPattern(const String& manufacturerData, const String& name, const String& address) {
  // Implement pattern matching for Lumos devices
  // This will be refined as we discover actual patterns
  
  // Check device name patterns
  if (name.indexOf("Lumos") >= 0 || name.indexOf("Matrix") >= 0) {
    return true;
  }
  
  // Check manufacturer data patterns
  // Add specific byte patterns here as they are discovered
  
  // Check MAC address patterns (if Lumos uses specific ranges)
  
  return false; // Default to false until patterns are discovered
}

String classifySignalPattern(const String& data, int rssi) {
  // Classify the type of signal based on data patterns
  
  if (data.length() == 0) return "no_data";
  
  // Add pattern classification logic here
  // Examples might include:
  // - "turn_left"
  // - "turn_right" 
  // - "brake"
  // - "hazard"
  // - "status"
  
  return "unclassified";
}

void updateDeviceList(const String& address, const String& name, int rssi, 
                     const String& manufacturerData, bool isLumos) {
  // Find existing device or add new one
  int deviceIndex = -1;
  for (int i = 0; i < deviceCount; i++) {
    if (detectedDevices[i].address == address) {
      deviceIndex = i;
      break;
    }
  }
  
  if (deviceIndex == -1 && deviceCount < MAX_DEVICES) {
    // Add new device
    deviceIndex = deviceCount++;
  }
  
  if (deviceIndex >= 0) {
    detectedDevices[deviceIndex].address = address;
    detectedDevices[deviceIndex].name = name;
    detectedDevices[deviceIndex].rssi = rssi;
    detectedDevices[deviceIndex].manufacturerData = manufacturerData;
    detectedDevices[deviceIndex].lastSeen = millis();
    detectedDevices[deviceIndex].signalCount++;
    detectedDevices[deviceIndex].isLumosDevice = isLumos;
  }
}

void addToSignalBuffer(const String& address, int rssi, const String& data, const String& pattern) {
  signalBuffer[signalBufferIndex].timestamp = millis();
  signalBuffer[signalBufferIndex].deviceAddress = address;
  signalBuffer[signalBufferIndex].rssi = rssi;
  signalBuffer[signalBufferIndex].data = data;
  signalBuffer[signalBufferIndex].patternType = pattern;
  
  signalBufferIndex = (signalBufferIndex + 1) % SIGNAL_BUFFER_SIZE;
}

void relaySignalToTrailer(const String& patternType, int rssi, const String& data) {
  // Send signal to trailer LED controller
  // This could be via Serial, WiFi, BLE, or other communication method
  
  Serial.printf("RELAY_TO_TRAILER: %s (RSSI: %d, Data: %s)\n", 
               patternType.c_str(), rssi, data.c_str());
  
  // TODO: Implement actual communication to trailer controller
}

void analyzeSignalPatterns() {
  Serial.println("=== Signal Pattern Analysis ===");
  
  // Count patterns in recent history
  unsigned long recentTime = millis() - 30000; // Last 30 seconds
  int patternCounts[10] = {0}; // Adjust size as needed
  
  for (int i = 0; i < SIGNAL_BUFFER_SIZE; i++) {
    if (signalBuffer[i].timestamp > recentTime) {
      // Count different pattern types
      // TODO: Implement pattern counting logic
    }
  }
  
  // Report active Lumos devices
  int activeLumosDevices = 0;
  for (int i = 0; i < deviceCount; i++) {
    if (detectedDevices[i].isLumosDevice && 
        (millis() - detectedDevices[i].lastSeen < 60000)) {
      activeLumosDevices++;
    }
  }
  
  Serial.printf("Active Lumos devices: %d\n", activeLumosDevices);
  Serial.printf("Total signals in buffer: %d\n", signalBufferIndex);
}

void setupWiFiAP() {
  WiFi.softAP("Lumos-BBR-Sniffer", "lumos123");
  Serial.println("WiFi AP started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
}

void setupWebServer() {
  // Serve main page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  
  // API endpoint for device list
  server.on("/api/devices", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "[";
    bool first = true;
    
    for (int i = 0; i < deviceCount; i++) {
      if (millis() - detectedDevices[i].lastSeen < 300000) { // Last 5 minutes
        if (!first) json += ",";
        json += "{";
        json += "\"address\":\"" + detectedDevices[i].address + "\",";
        json += "\"name\":\"" + detectedDevices[i].name + "\",";
        json += "\"rssi\":" + String(detectedDevices[i].rssi) + ",";
        json += "\"data\":\"" + detectedDevices[i].manufacturerData + "\",";
        json += "\"lastSeen\":" + String(detectedDevices[i].lastSeen) + ",";
        json += "\"signalCount\":" + String(detectedDevices[i].signalCount) + ",";
        json += "\"isLumos\":" + String(detectedDevices[i].isLumosDevice ? "true" : "false");
        json += "}";
        first = false;
      }
    }
    json += "]";
    
    request->send(200, "application/json", json);
  });
  
  // API endpoint for signal patterns
  server.on("/api/signals", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "[";
    bool first = true;
    
    // Return last 100 signals
    int startIndex = (signalBufferIndex - 100 + SIGNAL_BUFFER_SIZE) % SIGNAL_BUFFER_SIZE;
    for (int i = 0; i < 100; i++) {
      int index = (startIndex + i) % SIGNAL_BUFFER_SIZE;
      if (signalBuffer[index].timestamp > 0) {
        if (!first) json += ",";
        json += "{";
        json += "\"timestamp\":" + String(signalBuffer[index].timestamp) + ",";
        json += "\"address\":\"" + signalBuffer[index].deviceAddress + "\",";
        json += "\"rssi\":" + String(signalBuffer[index].rssi) + ",";
        json += "\"data\":\"" + signalBuffer[index].data + "\",";
        json += "\"pattern\":\"" + signalBuffer[index].patternType + "\"";
        json += "}";
        first = false;
      }
    }
    json += "]";
    
    request->send(200, "application/json", json);
  });
  
  // Serve static files
  server.serveStatic("/", SPIFFS, "/");
  
  server.begin();
  Serial.println("Web server started");
}