/*
 * Lumos BBR Trailer LED Controller
 * ESP32-based controller for bicycle trailer turn signals and brake lights
 * Receives signals from Lumos BLE sniffer and controls LED arrays
 * 
 * Features:
 * - Turn signal indicators (left/right)
 * - Brake light functionality
 * - Hazard light mode
 * - Battery monitoring
 * - WiFi communication with main sniffer
 * - Manual override switches
 * 
 * Author: noweheere
 * License: MIT
 */

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <FastLED.h>
#include <ArduinoJson.h>

// LED Configuration
#define NUM_LEDS_LEFT 20
#define NUM_LEDS_RIGHT 20
#define NUM_LEDS_BRAKE 30
#define LED_PIN_LEFT 18
#define LED_PIN_RIGHT 19
#define LED_PIN_BRAKE 21
#define LED_BRIGHTNESS 150
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

// Input pins
#define BUTTON_LEFT 12
#define BUTTON_RIGHT 13
#define BUTTON_HAZARD 14
#define BATTERY_SENSE A0

// Status LED
#define STATUS_LED 2

// WiFi Configuration
const char* ssid = "Lumos-BBR-Sniffer";
const char* password = "lumos123";

// LED Arrays
CRGB ledsLeft[NUM_LEDS_LEFT];
CRGB ledsRight[NUM_LEDS_RIGHT];
CRGB ledsBrake[NUM_LEDS_BRAKE];

// Web server
AsyncWebServer server(80);

// State variables
bool leftTurnActive = false;
bool rightTurnActive = false;
bool brakeActive = false;
bool hazardActive = false;
bool manualOverride = false;
unsigned long lastSignalTime = 0;
unsigned long turnSignalStartTime = 0;
int batteryLevel = 100;

// Pattern timing
unsigned long lastBlinkTime = 0;
bool blinkState = false;
const unsigned long BLINK_INTERVAL = 500; // ms
const unsigned long TURN_TIMEOUT = 10000; // 10 seconds auto-off
const unsigned long BRAKE_TIMEOUT = 5000; // 5 seconds auto-off

// Colors
CRGB COLOR_AMBER = CRGB(255, 100, 0);
CRGB COLOR_RED = CRGB(255, 0, 0);
CRGB COLOR_WHITE = CRGB(255, 255, 255);
CRGB COLOR_OFF = CRGB(0, 0, 0);

void setup() {
  Serial.begin(115200);
  Serial.println("=== Lumos BBR Trailer LED Controller Starting ===");
  
  // Initialize pins
  pinMode(STATUS_LED, OUTPUT);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_HAZARD, INPUT_PULLUP);
  pinMode(BATTERY_SENSE, INPUT);
  
  // Initialize LED strips
  FastLED.addLeds<LED_TYPE, LED_PIN_LEFT, COLOR_ORDER>(ledsLeft, NUM_LEDS_LEFT);
  FastLED.addLeds<LED_TYPE, LED_PIN_RIGHT, COLOR_ORDER>(ledsRight, NUM_LEDS_RIGHT);
  FastLED.addLeds<LED_TYPE, LED_PIN_BRAKE, COLOR_ORDER>(ledsBrake, NUM_LEDS_BRAKE);
  FastLED.setBrightness(LED_BRIGHTNESS);
  
  // Startup light sequence
  startupSequence();
  
  // Connect to WiFi
  setupWiFi();
  
  // Setup web server
  setupWebServer();
  
  Serial.println("Setup complete. Ready for signals.");
}

void loop() {
  // Check manual override buttons
  checkManualButtons();
  
  // Update battery level
  updateBatteryLevel();
  
  // Handle automatic timeouts
  handleTimeouts();
  
  // Update LED patterns
  updateLEDPatterns();
  
  // Update status LED
  updateStatusLED();
  
  delay(50);
}

void checkManualButtons() {
  static unsigned long lastButtonCheck = 0;
  if (millis() - lastButtonCheck < 100) return; // Debounce
  lastButtonCheck = millis();
  
  bool leftPressed = !digitalRead(BUTTON_LEFT);
  bool rightPressed = !digitalRead(BUTTON_RIGHT);
  bool hazardPressed = !digitalRead(BUTTON_HAZARD);
  
  if (hazardPressed) {
    hazardActive = !hazardActive;
    leftTurnActive = false;
    rightTurnActive = false;
    manualOverride = true;
    Serial.println("Manual hazard toggle");
    delay(200); // Prevent double-press
  } else if (leftPressed && !hazardActive) {
    leftTurnActive = !leftTurnActive;
    rightTurnActive = false;
    manualOverride = true;
    turnSignalStartTime = millis();
    Serial.println("Manual left turn toggle");
    delay(200);
  } else if (rightPressed && !hazardActive) {
    rightTurnActive = !rightTurnActive;
    leftTurnActive = false;
    manualOverride = true;
    turnSignalStartTime = millis();
    Serial.println("Manual right turn toggle");
    delay(200);
  }
}

void updateBatteryLevel() {
  static unsigned long lastBatteryCheck = 0;
  if (millis() - lastBatteryCheck < 5000) return; // Check every 5 seconds
  lastBatteryCheck = millis();
  
  int rawReading = analogRead(BATTERY_SENSE);
  // Convert to percentage (adjust based on your battery setup)
  batteryLevel = map(rawReading, 0, 4095, 0, 100);
  batteryLevel = constrain(batteryLevel, 0, 100);
}

void handleTimeouts() {
  unsigned long currentTime = millis();
  
  // Turn signal timeout (only if not manual override)
  if (!manualOverride && (leftTurnActive || rightTurnActive)) {
    if (currentTime - turnSignalStartTime > TURN_TIMEOUT) {
      leftTurnActive = false;
      rightTurnActive = false;
      Serial.println("Turn signal timeout");
    }
  }
  
  // Brake light timeout
  if (brakeActive && (currentTime - lastSignalTime > BRAKE_TIMEOUT)) {
    brakeActive = false;
    Serial.println("Brake light timeout");
  }
}

void updateLEDPatterns() {
  unsigned long currentTime = millis();
  
  // Update blink timing
  if (currentTime - lastBlinkTime > BLINK_INTERVAL) {
    blinkState = !blinkState;
    lastBlinkTime = currentTime;
  }
  
  // Clear all LEDs first
  fill_solid(ledsLeft, NUM_LEDS_LEFT, COLOR_OFF);
  fill_solid(ledsRight, NUM_LEDS_RIGHT, COLOR_OFF);
  fill_solid(ledsBrake, NUM_LEDS_BRAKE, COLOR_OFF);
  
  // Hazard lights (both turn signals)
  if (hazardActive) {
    if (blinkState) {
      fill_solid(ledsLeft, NUM_LEDS_LEFT, COLOR_AMBER);
      fill_solid(ledsRight, NUM_LEDS_RIGHT, COLOR_AMBER);
    }
  } else {
    // Left turn signal
    if (leftTurnActive && blinkState) {
      // Animated arrow pattern
      animatedArrowLeft();
    }
    
    // Right turn signal  
    if (rightTurnActive && blinkState) {
      // Animated arrow pattern
      animatedArrowRight();
    }
  }
  
  // Brake lights (always on top of turn signals)
  if (brakeActive) {
    fill_solid(ledsBrake, NUM_LEDS_BRAKE, COLOR_RED);
    
    // Add brake intensity to turn signals if active
    if (leftTurnActive) {
      for (int i = 0; i < NUM_LEDS_LEFT; i++) {
        ledsLeft[i] = blend(ledsLeft[i], COLOR_RED, 128);
      }
    }
    if (rightTurnActive) {
      for (int i = 0; i < NUM_LEDS_RIGHT; i++) {
        ledsRight[i] = blend(ledsRight[i], COLOR_RED, 128);
      }
    }
  }
  
  // Low battery warning (flash white)
  if (batteryLevel < 20 && blinkState) {
    for (int i = 0; i < NUM_LEDS_BRAKE; i += 5) {
      ledsBrake[i] = COLOR_WHITE;
    }
  }
  
  // Update physical LEDs
  FastLED.show();
}

void animatedArrowLeft() {
  // Create flowing arrow pattern from center to left
  static int arrowPos = 0;
  arrowPos = (arrowPos + 1) % NUM_LEDS_LEFT;
  
  for (int i = 0; i < NUM_LEDS_LEFT; i++) {
    int distance = abs(i - arrowPos);
    int brightness = max(0, 255 - (distance * 50));
    ledsLeft[i] = CRGB(brightness, brightness/2, 0); // Amber fade
  }
}

void animatedArrowRight() {
  // Create flowing arrow pattern from center to right
  static int arrowPos = 0;
  arrowPos = (arrowPos + 1) % NUM_LEDS_RIGHT;
  
  for (int i = 0; i < NUM_LEDS_RIGHT; i++) {
    int distance = abs(i - arrowPos);
    int brightness = max(0, 255 - (distance * 50));
    ledsRight[i] = CRGB(brightness, brightness/2, 0); // Amber fade
  }
}

void startupSequence() {
  Serial.println("Running startup LED sequence...");
  
  // Test each LED strip
  fill_solid(ledsLeft, NUM_LEDS_LEFT, COLOR_AMBER);
  FastLED.show();
  delay(500);
  
  fill_solid(ledsRight, NUM_LEDS_RIGHT, COLOR_AMBER);
  FastLED.show();
  delay(500);
  
  fill_solid(ledsBrake, NUM_LEDS_BRAKE, COLOR_RED);
  FastLED.show();
  delay(500);
  
  // All white briefly
  fill_solid(ledsLeft, NUM_LEDS_LEFT, COLOR_WHITE);
  fill_solid(ledsRight, NUM_LEDS_RIGHT, COLOR_WHITE);
  fill_solid(ledsBrake, NUM_LEDS_BRAKE, COLOR_WHITE);
  FastLED.show();
  delay(200);
  
  // Turn off
  fill_solid(ledsLeft, NUM_LEDS_LEFT, COLOR_OFF);
  fill_solid(ledsRight, NUM_LEDS_RIGHT, COLOR_OFF);
  fill_solid(ledsBrake, NUM_LEDS_BRAKE, COLOR_OFF);
  FastLED.show();
  
  Serial.println("Startup sequence complete");
}

void updateStatusLED() {
  static unsigned long lastStatusUpdate = 0;
  static bool statusState = false;
  
  if (millis() - lastStatusUpdate > 1000) {
    statusState = !statusState;
    digitalWrite(STATUS_LED, statusState);
    lastStatusUpdate = millis();
  }
}

void setupWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi connection failed. Running in standalone mode.");
  }
}

void setupWebServer() {
  // Status endpoint
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";
    json += "\"leftTurn\":" + String(leftTurnActive ? "true" : "false") + ",";
    json += "\"rightTurn\":" + String(rightTurnActive ? "true" : "false") + ",";
    json += "\"brake\":" + String(brakeActive ? "true" : "false") + ",";
    json += "\"hazard\":" + String(hazardActive ? "true" : "false") + ",";
    json += "\"battery\":" + String(batteryLevel) + ",";
    json += "\"manualOverride\":" + String(manualOverride ? "true" : "false");
    json += "}";
    request->send(200, "application/json", json);
  });
  
  // Signal control endpoint
  server.on("/signal", HTTP_POST, [](AsyncWebServerRequest *request){
    if (request->hasParam("type", true)) {
      String signalType = request->getParam("type", true)->value();
      handleIncomingSignal(signalType);
      request->send(200, "text/plain", "Signal processed");
    } else {
      request->send(400, "text/plain", "Missing signal type");
    }
  });
  
  // Manual control endpoints
  server.on("/control/left", HTTP_POST, [](AsyncWebServerRequest *request){
    leftTurnActive = !leftTurnActive;
    rightTurnActive = false;
    hazardActive = false;
    manualOverride = true;
    turnSignalStartTime = millis();
    request->send(200, "text/plain", "Left turn toggled");
  });
  
  server.on("/control/right", HTTP_POST, [](AsyncWebServerRequest *request){
    rightTurnActive = !rightTurnActive;
    leftTurnActive = false;
    hazardActive = false;
    manualOverride = true;
    turnSignalStartTime = millis();
    request->send(200, "text/plain", "Right turn toggled");
  });
  
  server.on("/control/hazard", HTTP_POST, [](AsyncWebServerRequest *request){
    hazardActive = !hazardActive;
    leftTurnActive = false;
    rightTurnActive = false;
    manualOverride = true;
    request->send(200, "text/plain", "Hazard toggled");
  });
  
  server.on("/control/off", HTTP_POST, [](AsyncWebServerRequest *request){
    leftTurnActive = false;
    rightTurnActive = false;
    brakeActive = false;
    hazardActive = false;
    manualOverride = false;
    request->send(200, "text/plain", "All signals off");
  });
  
  server.begin();
  Serial.println("Web server started on port 80");
}

void handleIncomingSignal(const String& signalType) {
  lastSignalTime = millis();
  manualOverride = false; // Clear manual override on incoming signal
  
  Serial.printf("Received signal: %s\n", signalType.c_str());
  
  if (signalType == "turn_left") {
    leftTurnActive = true;
    rightTurnActive = false;
    hazardActive = false;
    turnSignalStartTime = millis();
  } else if (signalType == "turn_right") {
    rightTurnActive = true;
    leftTurnActive = false;
    hazardActive = false;
    turnSignalStartTime = millis();
  } else if (signalType == "brake") {
    brakeActive = true;
  } else if (signalType == "hazard") {
    hazardActive = true;
    leftTurnActive = false;
    rightTurnActive = false;
  } else if (signalType == "off" || signalType == "status") {
    // Turn off turn signals but keep brake if active
    leftTurnActive = false;
    rightTurnActive = false;
    hazardActive = false;
  }
}