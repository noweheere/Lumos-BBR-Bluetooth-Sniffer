/*
 * Lumos BBR Trailer Relay Configuration
 * 
 * Configuration parameters for the ESP32 trailer relay system
 * Modify these values based on your trailer setup and requirements
 */

#ifndef TRAILER_RELAY_CONFIG_H
#define TRAILER_RELAY_CONFIG_H

// Hardware Pin Configuration
#define LEFT_INDICATOR_PIN 18          // Left turn indicator output
#define RIGHT_INDICATOR_PIN 19         // Right turn indicator output
#define BRAKE_LIGHT_PIN 21             // Brake light output
#define HAZARD_PIN_1 22                // Hazard light 1 (left side)
#define HAZARD_PIN_2 23                // Hazard light 2 (right side)
#define STATUS_LED_PIN 2               // Status LED
#define POWER_RELAY_PIN 25             // Main power relay control
#define HORN_PIN 26                    // Horn/buzzer output (optional)
#define REVERSE_LIGHT_PIN 27           // Reverse light output (optional)

// Additional Safety Pins
#define EMERGENCY_STOP_PIN 4           // Emergency stop input
#define BRAKE_INPUT_PIN 5              // Physical brake pedal input
#define MANUAL_OVERRIDE_PIN 12         // Manual override switch

// Timing Configuration
#define BLINK_RATE 500                 // Normal indicator blink rate (ms)
#define HAZARD_RATE 300                // Hazard light blink rate (ms) - faster
#define BRAKE_RESPONSE_TIME 50         // Brake light response time (ms)
#define COMMAND_TIMEOUT 3000           // Command timeout (ms)
#define EMERGENCY_BLINK_RATE 150       // Emergency blink rate (ms)

// Communication Configuration
#define SERIAL_BAUD_RATE 115200        // Serial communication speed
#define WIFI_TIMEOUT 10000             // WiFi connection timeout (ms)
#define ESPNOW_CHANNEL 1               // ESP-NOW communication channel

// Relay Control Configuration
#define RELAY_ACTIVE_HIGH true         // true if relay is active high, false if active low
#define PWM_FREQUENCY 1000             // PWM frequency for dimming (Hz)
#define PWM_RESOLUTION 8               // PWM resolution (bits)
#define MAX_BRIGHTNESS 255             // Maximum LED brightness (0-255)
#define MIN_BRIGHTNESS 50              // Minimum LED brightness (0-255)

// Safety Configuration
#define MAX_CONTINUOUS_ON_TIME 300000  // Max time any light can stay on (5 min)
#define FAILSAFE_TIMEOUT 10000         // Failsafe activation timeout (ms)
#define VOLTAGE_MONITOR_PIN A0         // Battery voltage monitoring pin
#define LOW_VOLTAGE_THRESHOLD 11.5     // Low voltage shutdown threshold (V)
#define OVERCURRENT_PROTECTION true    // Enable overcurrent protection

// Power Management
#define SLEEP_MODE_ENABLED true        // Enable automatic sleep mode
#define SLEEP_TIMEOUT 600000           // Sleep after 10 minutes of inactivity
#define WAKE_ON_SIGNAL true            // Wake up on received signal
#define DEEP_SLEEP_ENABLED false       // Use deep sleep (loses WiFi connection)

// Diagnostic Configuration
#define SELF_TEST_ON_STARTUP true      // Run self-test on startup
#define STATUS_REPORT_INTERVAL 30000   // Status report interval (ms)
#define LOG_ACTIVITY true              // Log all activity
#define TEMPERATURE_MONITORING true    // Monitor ESP32 temperature

// Signal Pattern Configuration
struct TrailerSignalConfig {
    // Blink patterns (number of blinks, pause duration)
    struct {
        int count;
        int onTime;
        int offTime;
    } leftTurn = {1, 500, 500};
    
    struct {
        int count;
        int onTime;
        int offTime;
    } rightTurn = {1, 500, 500};
    
    struct {
        int count;
        int onTime;
        int offTime;
    } hazard = {1, 300, 300};
    
    struct {
        int count;
        int onTime;
        int offTime;
    } emergency = {3, 150, 150};
};

// LED Configuration
struct LEDConfig {
    int brightness;
    bool usePWM;
    int fadeTime;
    bool temperatureCompensation;
};

// Default LED configurations for different outputs
#define LEFT_LED_CONFIG {255, true, 100, true}
#define RIGHT_LED_CONFIG {255, true, 100, true}
#define BRAKE_LED_CONFIG {255, false, 0, false}
#define HAZARD_LED_CONFIG {255, true, 50, true}

// Vehicle Integration
#define CANBUS_ENABLED false           // Enable CAN bus integration
#define CANBUS_SPEED 500000            // CAN bus speed (bps)
#define VEHICLE_VOLTAGE_12V true       // true for 12V systems, false for 24V

// Network Configuration
#define AP_MODE_ENABLED true           // Enable Access Point mode
#define AP_SSID "LumosTrailer"        // AP network name
#define AP_PASSWORD "trailer123"       // AP password
#define WEB_SERVER_ENABLED true        // Enable web configuration interface
#define WEB_SERVER_PORT 80             // Web server port

// MAC Address Configuration (replace with actual addresses)
// Main sniffer ESP32 MAC address
#define SNIFFER_MAC_ADDRESS {0x24, 0x6F, 0x28, 0x00, 0x00, 0x01}

// Backup Communication
#define BLUETOOTH_FALLBACK true        // Use Bluetooth if WiFi fails  
#define SERIAL_FALLBACK true           // Use Serial if wireless fails
#define RF_MODULE_ENABLED false        // Enable RF433/915MHz module

// Error Handling
#define MAX_RETRY_ATTEMPTS 3           // Maximum retry attempts for commands
#define ERROR_LED_ENABLED true         // Use status LED for error indication
#define BUZZER_ERROR_ALERTS false      // Use buzzer for error alerts
#define WATCHDOG_ENABLED true          // Enable hardware watchdog

// Calibration Values
#define VOLTAGE_DIVIDER_RATIO 0.2      // Voltage divider ratio for battery monitoring
#define CURRENT_SENSOR_SENSITIVITY 0.1 // Current sensor sensitivity (V/A)
#define TEMPERATURE_OFFSET 0.0         // Temperature sensor offset

// Features
#define SMART_BRIGHTNESS true          // Adjust brightness based on ambient light
#define SEQUENTIAL_INDICATORS false    // Sequential turn indicators (Euro style)
#define DOUBLE_FLASH_HAZARD true       // Double flash hazard pattern
#define BRAKE_PRIORITY true            // Brake lights override turn signals

// Version and Build Information
#define TRAILER_FIRMWARE_VERSION "1.0.0"
#define HARDWARE_REVISION "1.0"
#define COMPATIBILITY_VERSION "1.0"

#endif // TRAILER_RELAY_CONFIG_H