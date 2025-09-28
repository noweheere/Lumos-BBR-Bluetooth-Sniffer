# API Reference

This document describes the communication protocols, data formats, and programming interfaces for the Lumos BBR Bluetooth Sniffer system.

## Table of Contents

- [Serial Commands](#serial-commands)
- [ESP-NOW Protocol](#esp-now-protocol)
- [JSON Data Formats](#json-data-formats)
- [Signal Patterns](#signal-patterns)
- [Configuration API](#configuration-api)
- [Web Interface API](#web-interface-api)

## Serial Commands

Both ESP32 units accept text commands via serial interface for testing and debugging.

### Common Commands

| Command | Description | Response |
|---------|-------------|----------|
| `status` | Show system status | Status information display |
| `test` | Run hardware test sequence | Test results |
| `help` | Show available commands | Command list |
| `version` | Show firmware version | Version information |
| `reboot` | Restart the system | System restart |

### Sniffer ESP32 Commands

| Command | Description | Example |
|---------|-------------|---------|
| `scan_start` | Start BLE scanning | `scan_start` |
| `scan_stop` | Stop BLE scanning | `scan_stop` |
| `scan_time <seconds>` | Set scan duration | `scan_time 10` |
| `dump_history` | Show signal history | `dump_history` |
| `clear_history` | Clear stored signals | `clear_history` |
| `set_rssi <value>` | Set RSSI threshold | `set_rssi -70` |
| `debug_on` | Enable debug output | `debug_on` |
| `debug_off` | Disable debug output | `debug_off` |

### Trailer ESP32 Commands

| Command | Description | Example |
|---------|-------------|---------|
| `left_on` | Activate left indicator | `left_on` |
| `left_off` | Deactivate left indicator | `left_off` |
| `right_on` | Activate right indicator | `right_on` |
| `right_off` | Deactivate right indicator | `right_off` |
| `brake_on` | Activate brake lights | `brake_on` |
| `brake_off` | Deactivate brake lights | `brake_off` |
| `hazard_on` | Activate hazard lights | `hazard_on` |
| `hazard_off` | Deactivate hazard lights | `hazard_off` |
| `all_off` | Turn off all outputs | `all_off` |
| `brightness <0-255>` | Set LED brightness | `brightness 200` |
| `blink_rate <ms>` | Set blink timing | `blink_rate 400` |

## ESP-NOW Protocol

ESP-NOW provides wireless communication between the sniffer and trailer ESP32 units.

### Message Structure

```cpp
typedef struct {
    int signalType;        // Signal type identifier
    bool active;           // Signal state (on/off)
    unsigned long timestamp; // Timestamp of signal
    char message[32];      // Optional text message
    uint8_t checksum;      // Data integrity check
} CommandData;
```

### Signal Type Codes

| Code | Signal Type | Description |
|------|-------------|-------------|
| 0 | ALL_OFF | Turn off all indicators |
| 1 | LEFT_TURN | Left turn indicator |
| 2 | RIGHT_TURN | Right turn indicator |
| 3 | HAZARD | Hazard lights |
| 4 | BRAKE | Brake lights |
| 5 | REVERSE | Reverse lights (optional) |
| 6 | HORN | Horn/buzzer (optional) |

### Communication Flow

```
Sniffer ESP32                 Trailer ESP32
     │                             │
     ├── ESP-NOW Command ────────► │
     │   {type: 1, active: true}   │
     │                             ├── Activate Left Turn
     │                             │
     ├── ESP-NOW Command ────────► │
     │   {type: 0, active: false}  │
     │                             ├── Turn Off All
```

## JSON Data Formats

### Signal Analysis Output

The sniffer ESP32 outputs structured JSON data for signal analysis:

```json
{
  "timestamp": 1642784400000,
  "device": "24:6f:28:aa:bb:cc",
  "rssi": -65,
  "signal_type": "LEFT_TURN",
  "data": "01 00 01 00",
  "left_indicator": true,
  "right_indicator": false,
  "hazard_lights": false,
  "confidence": 0.95
}
```

### Status Information

System status is provided in JSON format:

```json
{
  "system": "bluetooth_sniffer",
  "version": "1.0.0",
  "uptime": 123456,
  "free_heap": 245760,
  "device_found": true,
  "lumos_address": "24:6f:28:aa:bb:cc",
  "scan_active": true,
  "signal_count": 15,
  "last_signal": 1642784400000
}
```

### Configuration Data

Configuration can be updated via JSON:

```json
{
  "ble_scan_time": 5,
  "rssi_threshold": -70,
  "signal_timeout": 3000,
  "debug_enabled": true,
  "blink_rate": 500,
  "brightness": 255
}
```

## Signal Patterns

### Lumos Protocol Analysis

The system analyzes BLE advertisement data to identify Lumos helmet signals.

#### Advertisement Data Structure

```
Byte 0: Signal Type
Byte 1: Sequence Number  
Byte 2: Status Flags
Byte 3: Checksum
```

#### Pattern Matching

```cpp
// Left turn pattern
uint8_t leftPattern[] = {0x01, 0x00, 0x01, 0x00};
uint8_t leftMask[] = {0xFF, 0x00, 0xFF, 0x00};

// Pattern matching function
bool matchPattern(const uint8_t* data, const uint8_t* pattern, 
                  const uint8_t* mask, int length) {
    for (int i = 0; i < length; i++) {
        if ((data[i] & mask[i]) != (pattern[i] & mask[i])) {
            return false;
        }
    }
    return true;
}
```

### Signal Confidence

The system calculates confidence scores for signal detection:

```cpp
struct SignalConfidence {
    float rssi_weight = 0.3;      // Signal strength importance
    float pattern_weight = 0.5;   // Pattern match importance  
    float timing_weight = 0.2;    // Timing consistency importance
    
    float calculate(int rssi, bool pattern_match, bool timing_valid) {
        float score = 0.0;
        
        // RSSI scoring (higher is better, up to -50 dBm)
        score += rssi_weight * max(0.0, min(1.0, (rssi + 100) / 50.0));
        
        // Pattern matching (binary)
        score += pattern_weight * (pattern_match ? 1.0 : 0.0);
        
        // Timing validation (binary)
        score += timing_weight * (timing_valid ? 1.0 : 0.0);
        
        return score;
    }
};
```

## Configuration API

### Runtime Configuration

Configuration parameters can be modified at runtime:

```cpp
// Get current configuration
void getConfig(JsonObject& config) {
    config["ble_scan_time"] = BLE_SCAN_TIME;
    config["rssi_threshold"] = MIN_RSSI_THRESHOLD;
    config["signal_timeout"] = SIGNAL_TIMEOUT;
    config["debug_enabled"] = DEBUG_ENABLED;
}

// Update configuration
bool setConfig(const JsonObject& config) {
    if (config.containsKey("ble_scan_time")) {
        BLE_SCAN_TIME = config["ble_scan_time"];
    }
    if (config.containsKey("rssi_threshold")) {
        MIN_RSSI_THRESHOLD = config["rssi_threshold"];
    }
    // ... additional parameters
    return true;
}
```

### Persistent Storage

Configuration is stored in ESP32 Preferences:

```cpp
#include <Preferences.h>

Preferences prefs;

void saveConfig() {
    prefs.begin("lumos-config", false);
    prefs.putInt("scan_time", BLE_SCAN_TIME);
    prefs.putInt("rssi_thresh", MIN_RSSI_THRESHOLD);
    prefs.putBool("debug", DEBUG_ENABLED);
    prefs.end();
}

void loadConfig() {
    prefs.begin("lumos-config", true);
    BLE_SCAN_TIME = prefs.getInt("scan_time", 5);
    MIN_RSSI_THRESHOLD = prefs.getInt("rssi_thresh", -70);
    DEBUG_ENABLED = prefs.getBool("debug", true);
    prefs.end();
}
```

## Web Interface API

### REST Endpoints

When web server is enabled, these endpoints are available:

#### GET /api/status
Returns system status information.

**Response:**
```json
{
  "status": "ok",
  "system": "trailer_relay",
  "uptime": 123456,
  "connections": 1,
  "outputs": {
    "left_indicator": false,
    "right_indicator": false,
    "brake_lights": false,
    "hazard_lights": false
  }
}
```

#### POST /api/command
Send control command to the system.

**Request:**
```json
{
  "command": "left_on",
  "duration": 5000
}
```

**Response:**
```json
{
  "status": "ok",
  "message": "Command executed"
}
```

#### GET /api/config
Get current configuration.

**Response:**
```json
{
  "blink_rate": 500,
  "brightness": 255,
  "timeout": 3000,
  "safety_enabled": true
}
```

#### PUT /api/config
Update configuration parameters.

**Request:**
```json
{
  "blink_rate": 400,
  "brightness": 200
}
```

### WebSocket Interface

Real-time communication via WebSocket on `/ws`:

```javascript
// Connect to WebSocket
const ws = new WebSocket('ws://192.168.4.1/ws');

// Send command
ws.send(JSON.stringify({
    type: 'command',
    signal: 'left_turn',
    active: true
}));

// Receive updates
ws.onmessage = function(event) {
    const data = JSON.parse(event.data);
    console.log('Status update:', data);
};
```

## Error Codes

### System Error Codes

| Code | Description | Action |
|------|-------------|--------|
| 100 | BLE initialization failed | Check ESP32 board support |
| 101 | ESP-NOW initialization failed | Verify WiFi configuration |
| 102 | Configuration load failed | Reset to defaults |
| 200 | Communication timeout | Check connections |
| 201 | Invalid command received | Verify command format |
| 300 | Hardware fault detected | Check wiring |
| 301 | Overcurrent protection | Reduce load |
| 400 | Emergency stop activated | Manual reset required |

### Debug Codes

Debug messages use standardized prefixes:

- `[DBG]` - General debug information
- `[BLE]` - Bluetooth-related messages
- `[COM]` - Communication events
- `[HW]` - Hardware status
- `[ERR]` - Error conditions
- `[WARN]` - Warning messages

## Programming Examples

### Custom Signal Handler

```cpp
class CustomSignalHandler {
public:
    void onSignalDetected(const SignalPattern& pattern) {
        // Custom processing logic
        if (pattern.signalType == "LEFT_TURN") {
            processLeftTurn(pattern);
        }
    }
    
private:
    void processLeftTurn(const SignalPattern& pattern) {
        // Implement custom left turn logic
        Serial.println("Custom left turn handler");
    }
};
```

### Advanced Pattern Matching

```cpp
class PatternAnalyzer {
public:
    bool analyzeSequence(const std::vector<uint8_t>& data) {
        // State machine for complex pattern analysis
        switch (currentState) {
            case WAITING:
                if (isStartPattern(data)) {
                    currentState = DETECTING;
                    return false;
                }
                break;
                
            case DETECTING:
                if (isSignalPattern(data)) {
                    currentState = CONFIRMED;
                    return true;
                }
                break;
                
            case CONFIRMED:
                // Process confirmed signal
                break;
        }
        return false;
    }
    
private:
    enum State { WAITING, DETECTING, CONFIRMED };
    State currentState = WAITING;
};
```

## Integration Examples

### MQTT Integration

```cpp
#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

void publishSignal(const SignalPattern& pattern) {
    StaticJsonDocument<200> doc;
    doc["type"] = pattern.signalType;
    doc["device"] = pattern.deviceAddress;
    doc["timestamp"] = pattern.timestamp;
    
    String message;
    serializeJson(doc, message);
    
    mqtt.publish("lumos/signals", message.c_str());
}
```

### CAN Bus Integration

```cpp
#include <CAN.h>

void sendCANMessage(int signalType, bool active) {
    uint8_t data[8] = {0};
    data[0] = signalType;
    data[1] = active ? 1 : 0;
    data[2] = (millis() >> 24) & 0xFF;
    data[3] = (millis() >> 16) & 0xFF;
    data[4] = (millis() >> 8) & 0xFF;
    data[5] = millis() & 0xFF;
    
    CAN.beginPacket(0x123); // CAN ID
    CAN.write(data, 6);
    CAN.endPacket();
}
```

## Performance Considerations

### Memory Management

- Signal history limited to 100 entries
- JSON documents use static allocation
- String operations minimized in interrupt handlers

### Timing Requirements

- BLE scan cycle: 5 seconds typical
- ESP-NOW latency: <10ms
- Light activation: <50ms
- Safety timeout: 3 seconds maximum

### Power Optimization

```cpp
// Enable light sleep between scans
void enterLightSleep(int duration_ms) {
    esp_sleep_enable_timer_wakeup(duration_ms * 1000);
    esp_light_sleep_start();
}

// Reduce CPU frequency for power savings
void setPowerMode(bool lowPower) {
    if (lowPower) {
        setCpuFrequencyMhz(80); // Reduce from 240MHz
    } else {
        setCpuFrequencyMhz(240); // Full speed
    }
}
```

## Security Considerations

### Data Validation

All received data should be validated:

```cpp
bool validateCommand(const CommandData& cmd) {
    // Check signal type range
    if (cmd.signalType < 0 || cmd.signalType > 6) {
        return false;
    }
    
    // Verify timestamp is recent
    unsigned long age = millis() - cmd.timestamp;
    if (age > 5000) { // 5 second max age
        return false;
    }
    
    // Validate checksum
    uint8_t calculated = calculateChecksum((uint8_t*)&cmd, sizeof(cmd) - 1);
    return calculated == cmd.checksum;
}
```

### Access Control

For web interface, implement basic authentication:

```cpp
bool authenticateUser(const String& username, const String& password) {
    // Simple authentication (use stronger methods in production)
    return (username == "admin" && password == "lumos123");
}
```

This API reference provides the foundation for extending and integrating the Lumos BBR Bluetooth Sniffer system with other applications and systems.