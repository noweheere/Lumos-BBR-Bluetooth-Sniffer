/*
 * LED Test Example
 * Test script for trailer LED controller hardware
 * 
 * This example tests:
 * - LED strip connectivity
 * - Color accuracy
 * - Animation patterns
 * - Button inputs
 * - Power consumption
 * 
 * Author: noweheere
 * License: MIT
 */

#include <FastLED.h>

// LED Configuration (adjust for your setup)
#define NUM_LEDS_LEFT 20
#define NUM_LEDS_RIGHT 20
#define NUM_LEDS_BRAKE 30
#define LED_PIN_LEFT 18
#define LED_PIN_RIGHT 19
#define LED_PIN_BRAKE 21
#define LED_BRIGHTNESS 150
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

// Button pins
#define BUTTON_LEFT 12
#define BUTTON_RIGHT 13
#define BUTTON_HAZARD 14

// LED Arrays
CRGB ledsLeft[NUM_LEDS_LEFT];
CRGB ledsRight[NUM_LEDS_RIGHT];
CRGB ledsBrake[NUM_LEDS_BRAKE];

// Colors
CRGB COLOR_AMBER = CRGB(255, 100, 0);
CRGB COLOR_RED = CRGB(255, 0, 0);
CRGB COLOR_WHITE = CRGB(255, 255, 255);
CRGB COLOR_GREEN = CRGB(0, 255, 0);
CRGB COLOR_BLUE = CRGB(0, 0, 255);
CRGB COLOR_OFF = CRGB(0, 0, 0);

int testPhase = 0;
unsigned long lastPhaseChange = 0;
const unsigned long PHASE_DURATION = 3000; // 3 seconds per test

void setup() {
  Serial.begin(115200);
  Serial.println("=== LED Test Starting ===");
  
  // Initialize buttons
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_HAZARD, INPUT_PULLUP);
  
  // Initialize LED strips
  FastLED.addLeds<LED_TYPE, LED_PIN_LEFT, COLOR_ORDER>(ledsLeft, NUM_LEDS_LEFT);
  FastLED.addLeds<LED_TYPE, LED_PIN_RIGHT, COLOR_ORDER>(ledsRight, NUM_LEDS_RIGHT);
  FastLED.addLeds<LED_TYPE, LED_PIN_BRAKE, COLOR_ORDER>(ledsBrake, NUM_LEDS_BRAKE);
  FastLED.setBrightness(LED_BRIGHTNESS);
  
  Serial.println("LED strips initialized");
  Serial.println("Starting automated test sequence...");
  Serial.println("Press any button to skip to manual control mode");
}

void loop() {
  // Check for button press to enter manual mode
  if (!digitalRead(BUTTON_LEFT) || !digitalRead(BUTTON_RIGHT) || !digitalRead(BUTTON_HAZARD)) {
    manualControlMode();
    return;
  }
  
  // Run automated test sequence
  if (millis() - lastPhaseChange > PHASE_DURATION) {
    testPhase++;
    lastPhaseChange = millis();
    
    if (testPhase > 10) {
      testPhase = 0; // Loop back to start
    }
  }
  
  runTestPhase(testPhase);
  FastLED.show();
  delay(50);
}

void runTestPhase(int phase) {
  // Clear all LEDs first
  fill_solid(ledsLeft, NUM_LEDS_LEFT, COLOR_OFF);
  fill_solid(ledsRight, NUM_LEDS_RIGHT, COLOR_OFF);
  fill_solid(ledsBrake, NUM_LEDS_BRAKE, COLOR_OFF);
  
  switch (phase) {
    case 0:
      Serial.println("Test Phase 0: All LEDs White");
      fill_solid(ledsLeft, NUM_LEDS_LEFT, COLOR_WHITE);
      fill_solid(ledsRight, NUM_LEDS_RIGHT, COLOR_WHITE);
      fill_solid(ledsBrake, NUM_LEDS_BRAKE, COLOR_WHITE);
      break;
      
    case 1:
      Serial.println("Test Phase 1: Left Turn Amber");
      fill_solid(ledsLeft, NUM_LEDS_LEFT, COLOR_AMBER);
      break;
      
    case 2:
      Serial.println("Test Phase 2: Right Turn Amber");
      fill_solid(ledsRight, NUM_LEDS_RIGHT, COLOR_AMBER);
      break;
      
    case 3:
      Serial.println("Test Phase 3: Brake Red");
      fill_solid(ledsBrake, NUM_LEDS_BRAKE, COLOR_RED);
      break;
      
    case 4:
      Serial.println("Test Phase 4: Left Blink Animation");
      blinkLeft();
      break;
      
    case 5:
      Serial.println("Test Phase 5: Right Blink Animation");
      blinkRight();
      break;
      
    case 6:
      Serial.println("Test Phase 6: Hazard Blink");
      blinkHazard();
      break;
      
    case 7:
      Serial.println("Test Phase 7: Rainbow Pattern");
      rainbowPattern();
      break;
      
    case 8:
      Serial.println("Test Phase 8: Individual LED Test");
      individualLEDTest();
      break;
      
    case 9:
      Serial.println("Test Phase 9: Power Test (All Red)");
      // Test maximum power consumption
      fill_solid(ledsLeft, NUM_LEDS_LEFT, COLOR_RED);
      fill_solid(ledsRight, NUM_LEDS_RIGHT, COLOR_RED);
      fill_solid(ledsBrake, NUM_LEDS_BRAKE, COLOR_RED);
      break;
      
    case 10:
      Serial.println("Test Phase 10: All Off (Power Save)");
      // All LEDs off - test power savings
      break;
  }
}

void blinkLeft() {
  static bool blinkState = false;
  static unsigned long lastBlink = 0;
  
  if (millis() - lastBlink > 500) {
    blinkState = !blinkState;
    lastBlink = millis();
  }
  
  if (blinkState) {
    fill_solid(ledsLeft, NUM_LEDS_LEFT, COLOR_AMBER);
  }
}

void blinkRight() {
  static bool blinkState = false;
  static unsigned long lastBlink = 0;
  
  if (millis() - lastBlink > 500) {
    blinkState = !blinkState;
    lastBlink = millis();
  }
  
  if (blinkState) {
    fill_solid(ledsRight, NUM_LEDS_RIGHT, COLOR_AMBER);
  }
}

void blinkHazard() {
  static bool blinkState = false;
  static unsigned long lastBlink = 0;
  
  if (millis() - lastBlink > 500) {
    blinkState = !blinkState;
    lastBlink = millis();
  }
  
  if (blinkState) {
    fill_solid(ledsLeft, NUM_LEDS_LEFT, COLOR_AMBER);
    fill_solid(ledsRight, NUM_LEDS_RIGHT, COLOR_AMBER);
  }
}

void rainbowPattern() {
  static uint8_t hue = 0;
  hue += 2;
  
  for (int i = 0; i < NUM_LEDS_LEFT; i++) {
    ledsLeft[i] = CHSV(hue + (i * 10), 255, 255);
  }
  for (int i = 0; i < NUM_LEDS_RIGHT; i++) {
    ledsRight[i] = CHSV(hue + (i * 10) + 120, 255, 255);
  }
  for (int i = 0; i < NUM_LEDS_BRAKE; i++) {
    ledsBrake[i] = CHSV(hue + (i * 5) + 240, 255, 255);
  }
}

void individualLEDTest() {
  static int currentLED = 0;
  static int currentStrip = 0;
  static unsigned long lastChange = 0;
  
  if (millis() - lastChange > 100) {
    // Light up one LED at a time
    if (currentStrip == 0) {
      ledsLeft[currentLED] = COLOR_GREEN;
      currentLED++;
      if (currentLED >= NUM_LEDS_LEFT) {
        currentLED = 0;
        currentStrip = 1;
      }
    } else if (currentStrip == 1) {
      ledsRight[currentLED] = COLOR_BLUE;
      currentLED++;
      if (currentLED >= NUM_LEDS_RIGHT) {
        currentLED = 0;
        currentStrip = 2;
      }
    } else if (currentStrip == 2) {
      ledsBrake[currentLED] = COLOR_WHITE;
      currentLED++;
      if (currentLED >= NUM_LEDS_BRAKE) {
        currentLED = 0;
        currentStrip = 0;
      }
    }
    
    lastChange = millis();
  }
}

void manualControlMode() {
  Serial.println("\n=== Manual Control Mode ===");
  Serial.println("Left Button: Toggle left turn");
  Serial.println("Right Button: Toggle right turn");
  Serial.println("Hazard Button: Toggle hazard lights");
  Serial.println("Hold all buttons for 3 seconds to exit");
  
  bool leftActive = false;
  bool rightActive = false;
  bool hazardActive = false;
  
  unsigned long allButtonsPressed = 0;
  
  while (true) {
    bool leftPressed = !digitalRead(BUTTON_LEFT);
    bool rightPressed = !digitalRead(BUTTON_RIGHT);
    bool hazardPressed = !digitalRead(BUTTON_HAZARD);
    
    // Check for exit condition
    if (leftPressed && rightPressed && hazardPressed) {
      if (allButtonsPressed == 0) {
        allButtonsPressed = millis();
      } else if (millis() - allButtonsPressed > 3000) {
        Serial.println("Exiting manual mode...");
        testPhase = 0;
        lastPhaseChange = millis();
        return;
      }
    } else {
      allButtonsPressed = 0;
    }
    
    // Handle button presses (with debouncing)
    static unsigned long lastButtonCheck = 0;
    if (millis() - lastButtonCheck > 200) {
      static bool lastLeftState = false;
      static bool lastRightState = false;
      static bool lastHazardState = false;
      
      if (leftPressed && !lastLeftState) {
        leftActive = !leftActive;
        rightActive = false;
        hazardActive = false;
        Serial.printf("Left turn: %s\n", leftActive ? "ON" : "OFF");
      }
      
      if (rightPressed && !lastRightState) {
        rightActive = !rightActive;
        leftActive = false;
        hazardActive = false;
        Serial.printf("Right turn: %s\n", rightActive ? "ON" : "OFF");
      }
      
      if (hazardPressed && !lastHazardState) {
        hazardActive = !hazardActive;
        leftActive = false;
        rightActive = false;
        Serial.printf("Hazard: %s\n", hazardActive ? "ON" : "OFF");
      }
      
      lastLeftState = leftPressed;
      lastRightState = rightPressed;
      lastHazardState = hazardPressed;
      lastButtonCheck = millis();
    }
    
    // Update LEDs based on current state
    fill_solid(ledsLeft, NUM_LEDS_LEFT, COLOR_OFF);
    fill_solid(ledsRight, NUM_LEDS_RIGHT, COLOR_OFF);
    fill_solid(ledsBrake, NUM_LEDS_BRAKE, COLOR_OFF);
    
    static bool blinkState = false;
    static unsigned long lastBlink = 0;
    if (millis() - lastBlink > 500) {
      blinkState = !blinkState;
      lastBlink = millis();
    }
    
    if (hazardActive && blinkState) {
      fill_solid(ledsLeft, NUM_LEDS_LEFT, COLOR_AMBER);
      fill_solid(ledsRight, NUM_LEDS_RIGHT, COLOR_AMBER);
    } else if (leftActive && blinkState) {
      fill_solid(ledsLeft, NUM_LEDS_LEFT, COLOR_AMBER);
    } else if (rightActive && blinkState) {
      fill_solid(ledsRight, NUM_LEDS_RIGHT, COLOR_AMBER);
    }
    
    FastLED.show();
    delay(50);
  }
}