/*
 * Lumos BBR Trailer Relay System
 * ESP32 companion code for trailer indicator control
 * 
 * This sketch receives commands from the main Bluetooth sniffer ESP32
 * and controls trailer indicator lights, brake lights, and hazard systems.
 * 
 * Author: noweheere
 * License: MIT
 */

#include <WiFi.h>
#include <esp_now.h>
#include <ArduinoJson.h>

// Pin definitions for trailer outputs
#define LEFT_INDICATOR_PIN 18
#define RIGHT_INDICATOR_PIN 19
#define BRAKE_LIGHT_PIN 21
#define HAZARD_PIN_1 22
#define HAZARD_PIN_2 23
#define STATUS_LED_PIN 2
#define POWER_RELAY_PIN 25

// Blink patterns
#define BLINK_RATE 500 // milliseconds
#define HAZARD_RATE 300 // milliseconds for hazard (faster)

// Global state variables
bool leftIndicatorActive = false;
bool rightIndicatorActive = false;
bool brakeActive = false;
bool hazardActive = false;
unsigned long lastBlinkTime = 0;
bool blinkState = false;
unsigned long lastCommandTime = 0;
bool connectionActive = false;

// Command timeout (turn off indicators after no signal)
#define COMMAND_TIMEOUT 3000 // 3 seconds

// ESP-NOW peer address (main sniffer ESP32)
uint8_t snifferMacAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Replace with actual MAC

// Data structure for commands
typedef struct {
  int signalType;    // 1=left, 2=right, 3=hazard, 4=brake
  bool active;       // true=on, false=off
  unsigned long timestamp;
  char message[32];
} CommandData;

CommandData lastCommand;

void setup() {
  Serial.begin(115200);
  Serial.println("Lumos BBR Trailer Relay System Starting...");
  
  // Initialize output pins
  pinMode(LEFT_INDICATOR_PIN, OUTPUT);
  pinMode(RIGHT_INDICATOR_PIN, OUTPUT);
  pinMode(BRAKE_LIGHT_PIN, OUTPUT);
  pinMode(HAZARD_PIN_1, OUTPUT);
  pinMode(HAZARD_PIN_2, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(POWER_RELAY_PIN, OUTPUT);
  
  // Turn off all outputs initially
  digitalWrite(LEFT_INDICATOR_PIN, LOW);
  digitalWrite(RIGHT_INDICATOR_PIN, LOW);
  digitalWrite(BRAKE_LIGHT_PIN, LOW);
  digitalWrite(HAZARD_PIN_1, LOW);
  digitalWrite(HAZARD_PIN_2, LOW);
  digitalWrite(STATUS_LED_PIN, LOW);
  digitalWrite(POWER_RELAY_PIN, HIGH); // Power relay normally on
  
  // Initialize WiFi in station mode for ESP-NOW
  WiFi.mode(WIFI_STA);
  
  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register receive callback
  esp_now_register_recv_cb(onDataRecv);
  
  // Add peer (main sniffer ESP32)
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, snifferMacAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  
  Serial.println("Trailer relay system initialized");
  Serial.println("Waiting for commands from sniffer...");
  
  // Status blink to show system is ready
  for (int i = 0; i < 3; i++) {
    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(200);
    digitalWrite(STATUS_LED_PIN, LOW);
    delay(200);
  }
}

void loop() {
  unsigned long currentTime = millis();
  
  // Handle timeout - turn off indicators if no recent commands
  if (currentTime - lastCommandTime > COMMAND_TIMEOUT) {
    if (leftIndicatorActive || rightIndicatorActive) {
      Serial.println("Command timeout - turning off indicators");
      leftIndicatorActive = false;
      rightIndicatorActive = false;
      // Keep brake lights and hazards until explicitly turned off
    }
    connectionActive = false;
  } else {
    connectionActive = true;
  }
  
  // Handle blinking patterns
  handleBlinking(currentTime);
  
  // Update status LED
  updateStatusLED(currentTime);
  
  // Check for serial commands (fallback communication method)
  checkSerialCommands();
  
  delay(50); // Small delay to prevent excessive CPU usage
}

// ESP-NOW receive callback
void onDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&lastCommand, incomingData, sizeof(lastCommand));
  
  Serial.printf("Received command: Type=%d, Active=%d, Time=%lu\n", 
                lastCommand.signalType, lastCommand.active, lastCommand.timestamp);
  
  processCommand(lastCommand);
  lastCommandTime = millis();
}

void processCommand(const CommandData& cmd) {
  switch (cmd.signalType) {
    case 1: // Left turn
      leftIndicatorActive = cmd.active;
      rightIndicatorActive = false; // Turn off opposite indicator
      Serial.println(cmd.active ? "Left indicator ON" : "Left indicator OFF");
      break;
      
    case 2: // Right turn
      rightIndicatorActive = cmd.active;
      leftIndicatorActive = false; // Turn off opposite indicator
      Serial.println(cmd.active ? "Right indicator ON" : "Right indicator OFF");
      break;
      
    case 3: // Hazard lights
      hazardActive = cmd.active;
      if (cmd.active) {
        leftIndicatorActive = false;
        rightIndicatorActive = false;
      }
      Serial.println(cmd.active ? "Hazard lights ON" : "Hazard lights OFF");
      break;
      
    case 4: // Brake lights
      brakeActive = cmd.active;
      digitalWrite(BRAKE_LIGHT_PIN, cmd.active ? HIGH : LOW);
      Serial.println(cmd.active ? "Brake lights ON" : "Brake lights OFF");
      break;
      
    case 0: // Turn off all
      leftIndicatorActive = false;
      rightIndicatorActive = false;
      hazardActive = false;
      brakeActive = false;
      digitalWrite(BRAKE_LIGHT_PIN, LOW);
      Serial.println("All indicators OFF");
      break;
      
    default:
      Serial.printf("Unknown command type: %d\n", cmd.signalType);
      break;
  }
}

void handleBlinking(unsigned long currentTime) {
  unsigned long blinkInterval = hazardActive ? HAZARD_RATE : BLINK_RATE;
  
  if (currentTime - lastBlinkTime >= blinkInterval) {
    blinkState = !blinkState;
    lastBlinkTime = currentTime;
    
    // Handle left indicator
    if (leftIndicatorActive || hazardActive) {
      digitalWrite(LEFT_INDICATOR_PIN, blinkState ? HIGH : LOW);
      digitalWrite(HAZARD_PIN_1, blinkState ? HIGH : LOW);
    } else {
      digitalWrite(LEFT_INDICATOR_PIN, LOW);
      digitalWrite(HAZARD_PIN_1, LOW);
    }
    
    // Handle right indicator
    if (rightIndicatorActive || hazardActive) {
      digitalWrite(RIGHT_INDICATOR_PIN, blinkState ? HIGH : LOW);
      digitalWrite(HAZARD_PIN_2, blinkState ? HIGH : LOW);
    } else {
      digitalWrite(RIGHT_INDICATOR_PIN, LOW);
      digitalWrite(HAZARD_PIN_2, LOW);
    }
  }
}

void updateStatusLED(unsigned long currentTime) {
  // Status LED patterns:
  // Solid ON = connected and receiving commands
  // Slow blink = waiting for connection
  // Fast blink = error state
  
  if (connectionActive) {
    digitalWrite(STATUS_LED_PIN, HIGH);
  } else {
    // Slow blink when not connected
    if ((currentTime / 1000) % 2 == 0) {
      digitalWrite(STATUS_LED_PIN, HIGH);
    } else {
      digitalWrite(STATUS_LED_PIN, LOW);
    }
  }
}

void checkSerialCommands() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    // Parse JSON commands from serial
    if (command.startsWith("TRAILER_CMD:")) {
      String jsonStr = command.substring(12); // Remove "TRAILER_CMD:" prefix
      
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, jsonStr);
      
      if (!error) {
        CommandData cmd;
        cmd.signalType = doc["signal"];
        cmd.active = true;
        cmd.timestamp = doc["timestamp"];
        
        processCommand(cmd);
        lastCommandTime = millis();
      } else {
        Serial.println("Error parsing JSON command");
      }
    }
    
    // Simple text commands for testing
    else if (command == "left_on") {
      CommandData cmd = {1, true, millis(), "left_on"};
      processCommand(cmd);
    }
    else if (command == "right_on") {
      CommandData cmd = {2, true, millis(), "right_on"};
      processCommand(cmd);
    }
    else if (command == "hazard_on") {
      CommandData cmd = {3, true, millis(), "hazard_on"};
      processCommand(cmd);
    }
    else if (command == "brake_on") {
      CommandData cmd = {4, true, millis(), "brake_on"};
      processCommand(cmd);
    }
    else if (command == "all_off") {
      CommandData cmd = {0, false, millis(), "all_off"};
      processCommand(cmd);
    }
    else if (command == "status") {
      printStatus();
    }
    else if (command == "test") {
      runTestSequence();
    }
  }
}

void printStatus() {
  Serial.println("=== TRAILER RELAY STATUS ===");
  Serial.printf("Connection active: %s\n", connectionActive ? "YES" : "NO");
  Serial.printf("Left indicator: %s\n", leftIndicatorActive ? "ACTIVE" : "OFF");
  Serial.printf("Right indicator: %s\n", rightIndicatorActive ? "ACTIVE" : "OFF");
  Serial.printf("Hazard lights: %s\n", hazardActive ? "ACTIVE" : "OFF");
  Serial.printf("Brake lights: %s\n", brakeActive ? "ACTIVE" : "OFF");
  Serial.printf("Last command time: %lu ms ago\n", millis() - lastCommandTime);
  Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
  Serial.println("============================");
}

void runTestSequence() {
  Serial.println("Running test sequence...");
  
  // Test all outputs
  Serial.println("Testing left indicator...");
  digitalWrite(LEFT_INDICATOR_PIN, HIGH);
  digitalWrite(HAZARD_PIN_1, HIGH);
  delay(1000);
  digitalWrite(LEFT_INDICATOR_PIN, LOW);
  digitalWrite(HAZARD_PIN_1, LOW);
  
  Serial.println("Testing right indicator...");
  digitalWrite(RIGHT_INDICATOR_PIN, HIGH);
  digitalWrite(HAZARD_PIN_2, HIGH);
  delay(1000);
  digitalWrite(RIGHT_INDICATOR_PIN, LOW);
  digitalWrite(HAZARD_PIN_2, LOW);
  
  Serial.println("Testing brake lights...");
  digitalWrite(BRAKE_LIGHT_PIN, HIGH);
  delay(1000);
  digitalWrite(BRAKE_LIGHT_PIN, LOW);
  
  Serial.println("Testing all together...");
  digitalWrite(LEFT_INDICATOR_PIN, HIGH);
  digitalWrite(RIGHT_INDICATOR_PIN, HIGH);
  digitalWrite(BRAKE_LIGHT_PIN, HIGH);
  digitalWrite(HAZARD_PIN_1, HIGH);
  digitalWrite(HAZARD_PIN_2, HIGH);
  delay(1000);
  
  // Turn everything off
  digitalWrite(LEFT_INDICATOR_PIN, LOW);
  digitalWrite(RIGHT_INDICATOR_PIN, LOW);
  digitalWrite(BRAKE_LIGHT_PIN, LOW);
  digitalWrite(HAZARD_PIN_1, LOW);
  digitalWrite(HAZARD_PIN_2, LOW);
  
  Serial.println("Test sequence complete");
}

// Emergency stop function
void emergencyStop() {
  digitalWrite(LEFT_INDICATOR_PIN, LOW);
  digitalWrite(RIGHT_INDICATOR_PIN, LOW);
  digitalWrite(BRAKE_LIGHT_PIN, LOW);
  digitalWrite(HAZARD_PIN_1, LOW);
  digitalWrite(HAZARD_PIN_2, LOW);
  digitalWrite(POWER_RELAY_PIN, LOW); // Cut main power
  
  leftIndicatorActive = false;
  rightIndicatorActive = false;
  brakeActive = false;
  hazardActive = false;
  
  Serial.println("EMERGENCY STOP ACTIVATED");
}

// Power management
void enterSleepMode() {
  Serial.println("Entering sleep mode...");
  
  // Turn off all outputs
  emergencyStop();
  
  // Configure wake-up sources
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0); // Wake on button press
  esp_sleep_enable_timer_wakeup(60 * 1000000); // Wake every minute
  
  esp_deep_sleep_start();
}