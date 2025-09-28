/*
 * Basic BLE Scanner Example
 * Simple ESP32 BLE scanner for testing and learning
 * 
 * This is a minimal version of the main sniffer for:
 * - Learning BLE basics
 * - Testing hardware setup
 * - Debugging connectivity issues
 * 
 * Author: noweheere
 * License: MIT
 */

#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEScan.h"
#include "BLEAdvertisedDevice.h"

#define SCAN_TIME 5  // seconds
#define LED_PIN 2

BLEScan* pBLEScan;
int deviceCount = 0;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    deviceCount++;
    
    Serial.printf("Device #%d:\n", deviceCount);
    Serial.printf("  Address: %s\n", advertisedDevice.getAddress().toString().c_str());
    Serial.printf("  Name: %s\n", advertisedDevice.getName().c_str());
    Serial.printf("  RSSI: %d dBm\n", advertisedDevice.getRSSI());
    
    // Show manufacturer data if available
    if (advertisedDevice.haveManufacturerData()) {
      std::string manData = advertisedDevice.getManufacturerData();
      Serial.printf("  Manufacturer Data: ");
      for (int i = 0; i < manData.length(); i++) {
        Serial.printf("%02X ", (unsigned char)manData[i]);
      }
      Serial.println();
    }
    
    // Show service UUID if available
    if (advertisedDevice.haveServiceUUID()) {
      Serial.printf("  Service UUID: %s\n", advertisedDevice.getServiceUUID().toString().c_str());
    }
    
    Serial.println("---");
    
    // Blink LED for each device found
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("=== Basic BLE Scanner Starting ===");
  Serial.println("This example scans for BLE devices every 5 seconds");
  Serial.println("Look for devices with 'Lumos' in the name or interesting data");
  Serial.println();
  
  // Initialize BLE
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  // Active scanning uses more power but gets more info
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);
  
  Serial.println("BLE initialized. Starting first scan...");
}

void loop() {
  Serial.printf("\n=== Starting BLE Scan (Scan #%d) ===\n", (millis() / (SCAN_TIME * 1000)) + 1);
  deviceCount = 0;
  
  // Start scan
  BLEScanResults foundDevices = pBLEScan->start(SCAN_TIME, false);
  
  Serial.printf("Scan complete. Found %d devices.\n", foundDevices.getCount());
  
  // Clear scan results to free memory
  pBLEScan->clearResults();
  
  // Wait before next scan
  Serial.println("Waiting 5 seconds before next scan...");
  for (int i = 5; i > 0; i--) {
    Serial.printf("%d...", i);
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
  }
  Serial.println("0");
}