# Configuration Guide

This guide explains how to configure the Lumos BBR Bluetooth Sniffer system for different use cases and hardware setups.

## Configuration Overview

The system uses multiple configuration files and methods:

- **Compile-time Configuration**: Header files in `config/` directory
- **Runtime Configuration**: Serial commands and web interface  
- **Persistent Storage**: ESP32 Preferences for settings that survive reboots
- **Profile-based Configuration**: Predefined setups for common scenarios

## Configuration Files

### Main Configuration Files

| File | Purpose | When to Modify |
|------|---------|----------------|
| `config/bluetooth_sniffer_config.h` | Sniffer ESP32 settings | Hardware changes, performance tuning |
| `config/trailer_relay_config.h` | Trailer ESP32 settings | Output pins, timing, safety parameters |
| `examples/basic_setup_example.h` | Template configuration | Starting point for new setups |

### Configuration Hierarchy

```
Default Values (in code)
    ↓
Configuration Headers (compile-time)
    ↓  
Stored Preferences (persistent)
    ↓
Runtime Commands (temporary)
```

## Bluetooth Sniffer Configuration

### Core BLE Settings

```cpp
// Basic scanning parameters
#define BLE_SCAN_TIME 5                // Scan duration (seconds)
#define BLE_SCAN_INTERVAL 100          // Scan interval (0.625ms units)
#define BLE_SCAN_WINDOW 99             // Scan window (0.625ms units)
#define BLE_ACTIVE_SCAN true           // Active vs passive scanning

// Signal filtering
#define MIN_RSSI_THRESHOLD -70         // Minimum signal strength (dBm)
#define DUPLICATE_FILTER_TIME 500      // Filter duplicates (ms)
#define SIGNAL_TIMEOUT 3000            // Signal timeout (ms)
```

### Hardware Pin Configuration

```cpp
// GPIO pin assignments
#define SNIFFER_LED_PIN 2              // Status LED
#define SNIFFER_RELAY_PIN 23           // Optional relay output
#define SNIFFER_BUTTON_PIN 0           // Mode/reset button
#define EMERGENCY_STOP_PIN 4           // Emergency stop input

// Communication pins (if using serial fallback)
#define SERIAL_TX_PIN 1                // Serial transmit
#define SERIAL_RX_PIN 3                // Serial receive
```

### Performance Tuning

```cpp
// Memory management
#define SIGNAL_HISTORY_SIZE 100        // Number of signals to store
#define MAX_SCAN_RESULTS 50            // Maximum scan results per cycle

// CPU and power optimization
#define MAIN_LOOP_DELAY 100            // Main loop delay (ms)
#define WATCHDOG_TIMEOUT 30000         // Watchdog timeout (ms)
#define LOW_POWER_MODE false           // Enable power saving
```

### Pattern Recognition Settings

```cpp
// Lumos device identification
#define LUMOS_SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define LUMOS_NAME_PATTERNS {"Lumos", "Matrix", "LUMOS", "Helmet"}

// Signal pattern matching
struct LumosSignalPatterns {
    uint8_t leftTurnPattern[4] = {0x01, 0x00, 0x01, 0x00};
    uint8_t rightTurnPattern[4] = {0x02, 0x00, 0x02, 0x00};
    uint8_t hazardPattern[4] = {0x03, 0x00, 0x03, 0x00};
    uint8_t brakePattern[4] = {0x00, 0x00, 0x00, 0x80};
    
    // Pattern confidence thresholds
    float minConfidence = 0.7;         // Minimum confidence for signal
    float falsePositiveThreshold = 0.3; // Maximum for false positive
};
```

## Trailer Relay Configuration

### Output Pin Configuration

```cpp
// Primary outputs
#define LEFT_INDICATOR_PIN 18          // Left turn signal
#define RIGHT_INDICATOR_PIN 19         // Right turn signal
#define BRAKE_LIGHT_PIN 21             // Brake lights
#define HAZARD_PIN_1 22                // Left hazard
#define HAZARD_PIN_2 23                // Right hazard

// Optional outputs
#define REVERSE_LIGHT_PIN 27           // Reverse lights
#define HORN_PIN 26                    // Horn/buzzer
#define AUXILIARY_PIN_1 28             // Extra output 1
#define AUXILIARY_PIN_2 29             // Extra output 2

// Control and status
#define STATUS_LED_PIN 2               // Status indicator
#define POWER_RELAY_PIN 25             // Main power control
```

### Timing Configuration

```cpp
// Blink patterns
#define BLINK_RATE 500                 // Normal turn signal rate (ms)
#define HAZARD_RATE 300                // Hazard light rate (ms)
#define EMERGENCY_BLINK_RATE 150       // Emergency pattern rate (ms)

// Response times
#define BRAKE_RESPONSE_TIME 50         // Brake light activation delay (ms)
#define SIGNAL_RESPONSE_TIME 100       // Turn signal activation delay (ms)

// Safety timeouts
#define COMMAND_TIMEOUT 3000           // Command timeout (ms)
#define MAX_CONTINUOUS_ON_TIME 300000  // Max continuous operation (5 min)
#define FAILSAFE_TIMEOUT 10000         // Failsafe activation (ms)
```

### Power and Safety Configuration

```cpp
// Power management
#define VOLTAGE_MONITOR_PIN A0         // Battery voltage monitoring
#define LOW_VOLTAGE_THRESHOLD 11.5     // Low voltage shutdown (V)
#define HIGH_VOLTAGE_THRESHOLD 14.5    // High voltage warning (V)

// Current monitoring
#define CURRENT_SENSOR_PIN A1          // Current sensor input
#define OVERCURRENT_THRESHOLD 8.0      // Overcurrent limit (A)
#define OVERCURRENT_PROTECTION true    // Enable protection

// Safety inputs
#define EMERGENCY_STOP_PIN 4           // Emergency stop button
#define MANUAL_OVERRIDE_PIN 12         // Manual override switch
#define BRAKE_INPUT_PIN 5              // Physical brake input
```

## Communication Configuration

### ESP-NOW Settings

```cpp
// ESP-NOW configuration
#define ESPNOW_CHANNEL 1               // WiFi channel for ESP-NOW
#define ESPNOW_ENCRYPT false           // Encryption (use for security)
#define ESPNOW_RETRY_ATTEMPTS 3        // Retry failed transmissions

// Peer MAC addresses (update with your actual addresses)
#define SNIFFER_MAC_ADDRESS {0x24, 0x6F, 0x28, 0x00, 0x00, 0x01}
#define TRAILER_MAC_ADDRESS {0x24, 0x6F, 0x28, 0x00, 0x00, 0x02}

// Communication timing
#define COMM_TIMEOUT 1000              // Communication timeout (ms)
#define HEARTBEAT_INTERVAL 5000        // Status heartbeat (ms)
#define MAX_MESSAGE_AGE 2000           // Maximum message age (ms)
```

### WiFi Configuration

```cpp
// WiFi settings (if using WiFi instead of ESP-NOW)
#define WIFI_SSID "LumosBBR"          // Network name
#define WIFI_PASSWORD "lumos123"       // Network password (change this!)
#define WIFI_CHANNEL 6                 // WiFi channel
#define WIFI_HIDDEN false              // Hidden network

// IP configuration
#define USE_STATIC_IP true             // Use static IP addresses
#define SNIFFER_IP IPAddress(192,168,4,1)
#define TRAILER_IP IPAddress(192,168,4,2)
#define GATEWAY_IP IPAddress(192,168,4,1)
#define SUBNET_MASK IPAddress(255,255,255,0)

// Server settings
#define WEB_SERVER_PORT 80             // Web interface port
#define UDP_PORT 4210                  // UDP communication port
#define TCP_PORT 4211                  // TCP communication port
```

## Configuration Profiles

### Profile 1: Basic Bicycle Trailer

```cpp
// Basic setup for simple bicycle trailer
#define PROFILE_BASIC_BICYCLE

#ifdef PROFILE_BASIC_BICYCLE
    // Simplified outputs
    #define LEFT_INDICATOR_PIN 18
    #define RIGHT_INDICATOR_PIN 19
    #define BRAKE_LIGHT_PIN 21
    
    // Conservative timing
    #define BLINK_RATE 600
    #define COMMAND_TIMEOUT 2000
    
    // Low power settings
    #define BLE_SCAN_TIME 3
    #define MAIN_LOOP_DELAY 200
    #define LOW_POWER_MODE true
    
    // Simple pattern matching
    #define MIN_CONFIDENCE_THRESHOLD 0.6
    #define SIGNAL_HISTORY_SIZE 25
#endif
```

### Profile 2: Commercial Vehicle

```cpp
// Setup for commercial vehicle with full features
#define PROFILE_COMMERCIAL_VEHICLE

#ifdef PROFILE_COMMERCIAL_VEHICLE
    // Full output configuration
    #define LEFT_INDICATOR_PIN 18
    #define RIGHT_INDICATOR_PIN 19
    #define BRAKE_LIGHT_PIN 21
    #define HAZARD_PIN_1 22
    #define HAZARD_PIN_2 23
    #define REVERSE_LIGHT_PIN 27
    #define HORN_PIN 26
    
    // Fast response timing
    #define BLINK_RATE 400
    #define BRAKE_RESPONSE_TIME 25
    #define COMMAND_TIMEOUT 1500
    
    // High performance scanning
    #define BLE_SCAN_TIME 2
    #define BLE_SCAN_INTERVAL 50
    #define MAIN_LOOP_DELAY 50
    
    // Advanced features
    #define WEB_SERVER_ENABLED true
    #define VOLTAGE_MONITORING true
    #define CURRENT_MONITORING true
    #define CAN_BUS_INTEGRATION true
#endif
```

### Profile 3: Development/Testing

```cpp
// Configuration for development and testing
#define PROFILE_DEVELOPMENT

#ifdef PROFILE_DEVELOPMENT
    // All debugging enabled
    #define DEBUG_ENABLED true
    #define LOG_ALL_DEVICES true
    #define LOG_RAW_DATA true
    #define VERBOSE_LOGGING true
    
    // Extended history and logging
    #define SIGNAL_HISTORY_SIZE 200
    #define LOG_BUFFER_SIZE 1000
    
    // Relaxed timing for testing
    #define COMMAND_TIMEOUT 10000
    #define BLE_SCAN_TIME 10
    
    // Test features
    #define SERIAL_COMMAND_INTERFACE true
    #define WEB_INTERFACE_ENABLED true
    #define REMOTE_DEBUG_ENABLED true
#endif
```

## Runtime Configuration

### Serial Commands for Configuration

```cpp
// Configuration commands via serial interface
"set_scan_time 5"           // Set BLE scan time to 5 seconds
"set_rssi -70"              // Set RSSI threshold to -70 dBm
"set_blink_rate 400"        // Set blink rate to 400ms
"set_timeout 3000"          // Set command timeout to 3 seconds
"set_brightness 200"        // Set LED brightness (0-255)

// Configuration queries
"get_config"                // Show all configuration
"get_scan_time"            // Show current scan time
"get_status"               // Show system status

// Save/load configuration
"save_config"              // Save current settings to flash
"load_config"              // Load settings from flash
"reset_config"             // Reset to defaults
```

### Web Interface Configuration

The web interface allows real-time configuration changes:

```html
<!-- Example web configuration form -->
<form id="configForm">
    <label>Scan Time (seconds):</label>
    <input type="number" id="scanTime" value="5" min="1" max="30">
    
    <label>RSSI Threshold (dBm):</label>
    <input type="number" id="rssiThreshold" value="-70" min="-100" max="-30">
    
    <label>Blink Rate (ms):</label>
    <input type="number" id="blinkRate" value="500" min="100" max="2000">
    
    <label>Command Timeout (ms):</label>
    <input type="number" id="commandTimeout" value="3000" min="1000" max="10000">
    
    <button type="submit">Update Configuration</button>
</form>
```

## Advanced Configuration

### Custom Pattern Recognition

```cpp
// Define custom signal patterns
struct CustomPatterns {
    // Custom left turn pattern
    PatternDefinition customLeft = {
        .pattern = {0x05, 0x01, 0xFF, 0x00},
        .mask = {0xFF, 0xFF, 0x00, 0xFF},
        .length = 4,
        .confidence = 0.8
    };
    
    // Custom brake pattern with multiple variants
    PatternDefinition customBrake[] = {
        {{0x00, 0x00, 0x80, 0x00}, {0xFF, 0xFF, 0xFF, 0x00}, 4, 0.9},
        {{0x00, 0x80, 0x00, 0x00}, {0xFF, 0xFF, 0xFF, 0x00}, 4, 0.8}
    };
};

// Custom pattern matching function
bool matchCustomPattern(const uint8_t* data, const PatternDefinition& pattern) {
    for (int i = 0; i < pattern.length; i++) {
        if ((data[i] & pattern.mask[i]) != pattern.pattern[i]) {
            return false;
        }
    }
    return true;
}
```

### Multi-Device Support

```cpp
// Configuration for multiple Lumos devices
struct MultiDeviceConfig {
    struct DeviceProfile {
        String macAddress;
        String deviceName;
        LumosSignalPatterns patterns;
        float confidenceThreshold;
        bool enabled;
    };
    
    DeviceProfile devices[5] = {
        {"24:6F:28:AA:BB:CC", "Helmet-1", defaultPatterns, 0.7, true},
        {"24:6F:28:DD:EE:FF", "Helmet-2", customPatterns, 0.8, true}
    };
    
    int maxDevices = 5;
    bool autoDiscovery = true;
    int discoveryTimeout = 30000;  // 30 seconds
};
```

### Performance Optimization

```cpp
// CPU frequency scaling based on load
void adjustCPUFrequency(int load_percentage) {
    if (load_percentage > 80) {
        setCpuFrequencyMhz(240);  // Full speed
    } else if (load_percentage > 50) {
        setCpuFrequencyMhz(160);  // Medium speed
    } else {
        setCpuFrequencyMhz(80);   // Low speed
    }
}

// Dynamic scan time adjustment
void adjustScanTime(int signal_strength, int device_count) {
    if (signal_strength > -50 && device_count < 5) {
        BLE_SCAN_TIME = 2;  // Fast scanning for strong signals
    } else if (signal_strength < -80 || device_count > 20) {
        BLE_SCAN_TIME = 10; // Slow scanning for weak signals
    } else {
        BLE_SCAN_TIME = 5;  // Default
    }
}
```

## Configuration Validation

### Input Validation

```cpp
bool validateConfiguration(const Config& config) {
    // Validate scan time
    if (config.ble_scan_time < 1 || config.ble_scan_time > 30) {
        Serial.println("ERROR: Scan time must be 1-30 seconds");
        return false;
    }
    
    // Validate RSSI threshold
    if (config.rssi_threshold > -30 || config.rssi_threshold < -100) {
        Serial.println("ERROR: RSSI threshold must be -100 to -30 dBm");
        return false;
    }
    
    // Validate GPIO pins
    if (!isValidGPIO(config.left_indicator_pin)) {
        Serial.println("ERROR: Invalid left indicator pin");
        return false;
    }
    
    // Validate timing parameters
    if (config.blink_rate < 100 || config.blink_rate > 2000) {
        Serial.println("ERROR: Blink rate must be 100-2000 ms");
        return false;
    }
    
    return true;
}

bool isValidGPIO(int pin) {
    // Check if pin is available for output
    int invalidPins[] = {6, 7, 8, 9, 10, 11}; // Flash pins
    for (int i = 0; i < sizeof(invalidPins)/sizeof(int); i++) {
        if (pin == invalidPins[i]) return false;
    }
    return (pin >= 0 && pin <= 39);
}
```

### Configuration Backup and Restore

```cpp
// Backup configuration to JSON file
void backupConfiguration() {
    StaticJsonDocument<1024> doc;
    
    // Serialize current configuration
    doc["version"] = FIRMWARE_VERSION;
    doc["timestamp"] = millis();
    doc["ble_scan_time"] = BLE_SCAN_TIME;
    doc["rssi_threshold"] = MIN_RSSI_THRESHOLD;
    doc["blink_rate"] = BLINK_RATE;
    doc["command_timeout"] = COMMAND_TIMEOUT;
    
    // Save to file or send via serial
    String output;
    serializeJsonPretty(doc, output);
    Serial.println("CONFIG_BACKUP: " + output);
}

// Restore configuration from JSON
bool restoreConfiguration(const String& jsonConfig) {
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, jsonConfig);
    
    if (error) {
        Serial.println("ERROR: Invalid configuration JSON");
        return false;
    }
    
    // Validate and apply configuration
    if (doc.containsKey("ble_scan_time")) {
        int value = doc["ble_scan_time"];
        if (value >= 1 && value <= 30) {
            BLE_SCAN_TIME = value;
        }
    }
    
    // Apply other settings...
    return true;
}
```

## Configuration Best Practices

### Development Phase

1. **Start with basic profile** and test fundamental functionality
2. **Enable verbose logging** to understand system behavior
3. **Use relaxed timing** to avoid false errors during development
4. **Test one feature at a time** when adding complexity

### Production Deployment

1. **Disable debug logging** to improve performance
2. **Set appropriate timeouts** for your environment
3. **Enable all safety features** including watchdog and emergency stop
4. **Test thoroughly** with actual hardware loads

### Performance Tuning

1. **Monitor memory usage** and adjust history buffer sizes
2. **Optimize scan parameters** based on signal environment
3. **Adjust CPU frequency** based on processing requirements
4. **Use power management** for battery-powered applications

### Security Considerations

1. **Change default passwords** for web interfaces
2. **Use strong encryption** for wireless communications
3. **Validate all inputs** from external sources
4. **Keep firmware updated** with security patches

## Troubleshooting Configuration Issues

### Common Configuration Problems

**Problem**: System doesn't detect Lumos device
- Check MAC address patterns in configuration
- Verify RSSI threshold isn't too restrictive
- Enable debug logging to see detected devices

**Problem**: Lights don't activate
- Verify GPIO pin assignments
- Check power supply and connections
- Test with manual commands first

**Problem**: Communication failures between ESP32s
- Confirm MAC addresses are correct
- Check WiFi channel conflicts
- Verify power supply stability

**Problem**: System crashes or resets
- Check memory usage and buffer sizes
- Verify watchdog timeout settings
- Monitor for stack overflow conditions

For additional support, see the [Testing Guide](testing.md) and [Troubleshooting section](installation.md#troubleshooting).