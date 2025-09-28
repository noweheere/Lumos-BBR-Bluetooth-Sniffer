/*
 * Lumos BBR Bluetooth Sniffer Configuration
 * 
 * Configuration parameters for the ESP32 Bluetooth sniffer
 * Modify these values based on your specific setup and requirements
 */

#ifndef BLUETOOTH_SNIFFER_CONFIG_H
#define BLUETOOTH_SNIFFER_CONFIG_H

// Hardware Configuration
#define SNIFFER_LED_PIN 2              // Status LED pin
#define SNIFFER_RELAY_PIN 23           // Main relay output pin
#define SNIFFER_BUTTON_PIN 0           // Reset/mode button
#define SNIFFER_SERIAL_BAUD 115200     // Serial communication speed

// BLE Scanning Configuration
#define BLE_SCAN_TIME 5                // Scan duration in seconds
#define BLE_SCAN_INTERVAL 100          // Scan interval (0.625ms units)
#define BLE_SCAN_WINDOW 99             // Scan window (0.625ms units)
#define BLE_ACTIVE_SCAN true           // Use active scanning

// Lumos Device Identification
#define LUMOS_SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define LUMOS_CHAR_UUID_TX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define LUMOS_CHAR_UUID_RX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// Known Lumos device name patterns
#define LUMOS_NAME_PATTERNS {"Lumos", "Matrix", "LUMOS", "Helmet"}

// Signal Analysis Configuration
#define SIGNAL_TIMEOUT 5000            // Signal timeout in milliseconds
#define SIGNAL_HISTORY_SIZE 100        // Number of signals to store in history
#define MIN_RSSI_THRESHOLD -80         // Minimum RSSI to consider valid signal
#define DUPLICATE_FILTER_TIME 500      // Filter duplicate signals (ms)

// Communication Configuration
#define USE_WIFI_COMMUNICATION true    // Enable WiFi for trailer communication
#define USE_ESPNOW_COMMUNICATION true  // Enable ESP-NOW
#define WIFI_SSID "LumosBBR"          // WiFi network name
#define WIFI_PASSWORD "lumos123"       // WiFi password (change this!)
#define TRAILER_IP "192.168.4.2"      // Trailer ESP32 IP address

// Protocol Analysis Patterns
// These patterns need to be determined through actual Lumos device analysis
struct LumosSignalPatterns {
    // Turn signal patterns (example values - need real analysis)
    uint8_t leftTurnPattern[4] = {0x01, 0x00, 0x01, 0x00};
    uint8_t rightTurnPattern[4] = {0x02, 0x00, 0x02, 0x00};
    uint8_t hazardPattern[4] = {0x03, 0x00, 0x03, 0x00};
    uint8_t brakePattern[4] = {0x00, 0x00, 0x00, 0x80};
    
    // Pattern masks for flexible matching
    uint8_t leftTurnMask[4] = {0xFF, 0x00, 0xFF, 0x00};
    uint8_t rightTurnMask[4] = {0xFF, 0x00, 0xFF, 0x00};
    uint8_t hazardMask[4] = {0xFF, 0x00, 0xFF, 0x00};
    uint8_t brakeMask[4] = {0x00, 0x00, 0x00, 0xFF};
};

// Debug and Logging Configuration
#define DEBUG_ENABLED true             // Enable debug output
#define LOG_ALL_DEVICES false          // Log all BLE devices (not just Lumos)
#define LOG_RAW_DATA true              // Log raw advertisement data
#define LOG_JSON_FORMAT true           // Output structured JSON logs

// Performance Configuration
#define MAIN_LOOP_DELAY 100            // Main loop delay (ms)
#define WATCHDOG_TIMEOUT 30000         // Watchdog timeout (ms)
#define MAX_SCAN_RESULTS 50            // Maximum scan results to process

// Safety Configuration 
#define AUTO_SHUTOFF_TIME 300000       // Auto shut-off after 5 minutes of no activity
#define EMERGENCY_STOP_PIN 4           // Emergency stop input pin
#define FAILSAFE_MODE true             // Enable failsafe operations

// Version Information
#define FIRMWARE_VERSION "1.0.0"
#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

#endif // BLUETOOTH_SNIFFER_CONFIG_H