/*
 * Basic Setup Example Configuration
 * 
 * This is a basic configuration example for getting started with
 * the Lumos BBR Bluetooth Sniffer system. Copy these settings
 * to your configuration files and modify as needed.
 */

#ifndef BASIC_SETUP_EXAMPLE_H
#define BASIC_SETUP_EXAMPLE_H

// ============================================================================
// BLUETOOTH SNIFFER CONFIGURATION EXAMPLE
// ============================================================================

// Basic Hardware Pins
#define SNIFFER_LED_PIN 2              // Built-in LED on most ESP32 boards
#define SNIFFER_RELAY_PIN 23           // General purpose output pin
#define SNIFFER_BUTTON_PIN 0           // Built-in BOOT button
#define SNIFFER_SERIAL_BAUD 115200     // Standard baud rate

// Basic BLE Settings
#define BLE_SCAN_TIME 5                // 5 second scans
#define BLE_SCAN_INTERVAL 100          // Standard interval
#define BLE_SCAN_WINDOW 99             // Nearly continuous scanning
#define BLE_ACTIVE_SCAN true           // Active scanning for more data

// Simple Signal Analysis
#define SIGNAL_TIMEOUT 3000            // 3 second timeout
#define SIGNAL_HISTORY_SIZE 50         // Store last 50 signals
#define MIN_RSSI_THRESHOLD -70         // Minimum signal strength

// Basic Communication
#define USE_WIFI_COMMUNICATION false   // Disable WiFi for simplicity
#define USE_ESPNOW_COMMUNICATION true  // Use ESP-NOW only
#define TRAILER_MAC_ADDRESS {0x24, 0x6F, 0x28, 0x00, 0x00, 0x02} // Update this!

// Debug Settings
#define DEBUG_ENABLED true             // Enable debug output
#define LOG_ALL_DEVICES false          // Only log Lumos devices
#define LOG_RAW_DATA true              // Log raw data for analysis

// ============================================================================
// TRAILER RELAY CONFIGURATION EXAMPLE  
// ============================================================================

// Basic Output Pins
#define LEFT_INDICATOR_PIN 18          // Left turn signal
#define RIGHT_INDICATOR_PIN 19         // Right turn signal  
#define BRAKE_LIGHT_PIN 21             // Brake lights
#define HAZARD_PIN_1 22                // Left hazard
#define HAZARD_PIN_2 23                // Right hazard
#define STATUS_LED_PIN 2               // Built-in status LED
#define POWER_RELAY_PIN 25             // Main power control

// Basic Timing
#define BLINK_RATE 500                 // 500ms on/off for turn signals
#define HAZARD_RATE 300                // 300ms on/off for hazards (faster)
#define COMMAND_TIMEOUT 3000           // 3 second command timeout

// Basic Safety
#define EMERGENCY_STOP_PIN 4           // Emergency stop button
#define FAILSAFE_TIMEOUT 5000          // 5 second failsafe
#define MAX_CONTINUOUS_ON_TIME 300000  // 5 minutes max continuous operation

// Basic Communication
#define SERIAL_BAUD_RATE 115200        // Match sniffer baud rate
#define SNIFFER_MAC_ADDRESS {0x24, 0x6F, 0x28, 0x00, 0x00, 0x01} // Update this!

// ============================================================================
// CONNECTION EXAMPLE
// ============================================================================

/*
 * Basic Wiring Example:
 * 
 * ESP32 Sniffer:
 * - GPIO2  -> Built-in LED (status)
 * - GPIO23 -> Optional relay or LED (signal detected)
 * - 5V     -> Power supply
 * - GND    -> Ground
 * 
 * ESP32 Trailer:
 * - GPIO18 -> Left indicator (via MOSFET driver)
 * - GPIO19 -> Right indicator (via MOSFET driver)
 * - GPIO21 -> Brake lights (via MOSFET driver)
 * - GPIO22 -> Left hazard (via MOSFET driver)
 * - GPIO23 -> Right hazard (via MOSFET driver)
 * - GPIO2  -> Built-in LED (status)
 * - GPIO25 -> Power relay (optional)
 * - 5V     -> Regulated from 12V vehicle power
 * - GND    -> Vehicle ground
 * 
 * MOSFET Driver (per output):
 * - ESP32 GPIO -> 2.2K resistor -> MOSFET Gate
 * - MOSFET Gate -> 10K resistor -> Ground
 * - MOSFET Drain -> LED Strip positive
 * - MOSFET Source -> Ground
 * - LED Strip negative -> Vehicle 12V
 */

// ============================================================================
// SIMPLE TEST PROCEDURE
// ============================================================================

/*
 * Quick Test Steps:
 * 
 * 1. Upload sniffer code to first ESP32
 * 2. Upload trailer code to second ESP32  
 * 3. Connect both to power and serial monitors
 * 4. Both should boot and show status messages
 * 5. On trailer ESP32, send command: test
 * 6. Should see LED test sequence
 * 7. On trailer ESP32, send command: left_on
 * 8. Left indicator should start blinking
 * 9. Send command: all_off to stop
 * 
 * If this works, the basic system is functional!
 */

// ============================================================================
// MAC ADDRESS DISCOVERY
// ============================================================================

/*
 * To find your ESP32 MAC addresses, upload this simple sketch:
 * 
 * #include "WiFi.h"
 * void setup() {
 *   Serial.begin(115200);
 *   WiFi.mode(WIFI_MODE_STA);
 *   Serial.print("MAC Address: ");
 *   Serial.println(WiFi.macAddress());
 * }
 * void loop() {}
 * 
 * Then update the MAC addresses in the configuration files.
 */

// ============================================================================
// TROUBLESHOOTING BASICS
// ============================================================================

/*
 * Common Issues:
 * 
 * 1. "No devices found" - Check BLE is enabled and working
 * 2. "Communication failed" - Verify MAC addresses are correct
 * 3. "LEDs not working" - Check power supply and connections
 * 4. "Upload failed" - Hold BOOT button during upload
 * 5. "Crashes/resets" - Check power supply capacity
 * 
 * Debug Commands (send via Serial Monitor):
 * - status: Show system status
 * - test: Run hardware test
 * - left_on: Turn on left indicator
 * - right_on: Turn on right indicator
 * - all_off: Turn off all outputs
 */

#endif // BASIC_SETUP_EXAMPLE_H