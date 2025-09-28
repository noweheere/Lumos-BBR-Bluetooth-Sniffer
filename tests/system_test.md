# 🧪 System Testing Guide

This document provides comprehensive testing procedures for the Lumos BBR Bluetooth Sniffer system.

## 📋 Pre-Test Checklist

### Hardware Verification
- [ ] ESP32 boards programmed and functional
- [ ] LED strips connected and tested
- [ ] Buttons wired and responsive
- [ ] Power supplies adequate for all components
- [ ] All connections secure and insulated
- [ ] Enclosures weather-sealed if needed

### Software Verification
- [ ] Arduino IDE with ESP32 support installed
- [ ] Required libraries installed (BLE, FastLED, ESPAsyncWebServer, ArduinoJson)
- [ ] Code compiles without errors
- [ ] SPIFFS data uploaded successfully
- [ ] Serial monitor functional

## 🔧 Unit Tests

### Test 1: Basic BLE Scanner

**Purpose**: Verify BLE scanning functionality
**Hardware**: ESP32 with BLE sniffer code
**Duration**: 5 minutes

**Procedure**:
1. Upload `examples/basic_ble_scanner.ino`
2. Open serial monitor (115200 baud)
3. Observe device detection for 5 minutes
4. Use smartphone with BLE enabled as test device

**Expected Results**:
```
=== Basic BLE Scanner Starting ===
Device #1:
  Address: AA:BB:CC:DD:EE:FF
  Name: iPhone
  RSSI: -45 dBm
  Manufacturer Data: 4C 00 01 02 03
---
```

**Pass Criteria**:
- [ ] At least 3 different devices detected
- [ ] RSSI values reasonable (-30 to -80 dBm)
- [ ] Manufacturer data displayed correctly
- [ ] No memory leaks or crashes after 5 minutes

### Test 2: LED Controller Hardware

**Purpose**: Verify LED strips and buttons
**Hardware**: ESP32 with LED controller code
**Duration**: 10 minutes

**Procedure**:
1. Upload `examples/led_test.ino`
2. Let automated test sequence run (30 seconds total)
3. Press any button to enter manual mode
4. Test each button function
5. Verify LED colors and patterns

**Expected Results**:
- All LEDs light up in white during phase 0
- Individual strips work correctly
- Button inputs responsive
- No flickering or dead LEDs

**Pass Criteria**:
- [ ] All LED phases display correctly
- [ ] Left/right/hazard buttons functional
- [ ] Colors accurate (amber for turns, red for brake)
- [ ] No overheating of components

### Test 3: WiFi Communication

**Purpose**: Verify wireless communication between units
**Hardware**: Both ESP32 units
**Duration**: 15 minutes

**Procedure**:
1. Power on BLE sniffer (creates WiFi AP)
2. Wait 30 seconds for network establishment
3. Power on LED controller
4. Verify connection via serial monitors
5. Test web interface access

**Expected Results**:
```
BLE Sniffer:
WiFi AP started
IP address: 192.168.4.1
Web server started

LED Controller:
WiFi connected!
IP address: 192.168.4.2
```

**Pass Criteria**:
- [ ] WiFi AP created successfully
- [ ] LED controller connects automatically
- [ ] Web interface accessible at 192.168.4.1
- [ ] Device list updates in web interface

## 🔄 Integration Tests

### Test 4: End-to-End Signal Flow

**Purpose**: Test complete signal chain from BLE detection to LED activation
**Hardware**: Complete system setup
**Duration**: 20 minutes

**Procedure**:
1. Start both units and verify WiFi connection
2. Use smartphone with BLE app to generate test signals
3. Monitor BLE sniffer for device detection
4. Manually trigger signals via web interface
5. Verify LED controller responds correctly

**Test Scenarios**:
```bash
# Test 1: Manual Web Control
1. Access web interface
2. Navigate to trailer control section
3. Click "Left Turn" - verify left LEDs blink amber
4. Click "Right Turn" - verify right LEDs blink amber
5. Click "Hazard" - verify both sides blink amber
6. Click "Off" - verify all signals stop

# Test 2: Simulated BLE Signals
1. Modify pattern detection to accept test device
2. Have assistant use phone near sniffer
3. Generate manufacturer data that matches patterns
4. Verify LED controller receives and displays signals
```

**Pass Criteria**:
- [ ] Web interface controls work correctly
- [ ] Signal latency < 500ms from trigger to LED
- [ ] No false positives or missed signals
- [ ] System recovers gracefully from errors

### Test 5: Pattern Recognition Accuracy

**Purpose**: Test BLE pattern detection accuracy
**Hardware**: BLE sniffer with test data
**Duration**: 30 minutes

**Procedure**:
1. Create test dataset with known BLE patterns
2. Feed patterns to classification system
3. Measure accuracy, precision, and recall
4. Test with edge cases and noise

**Test Data Format**:
```cpp
struct TestPattern {
    String input;           // Raw manufacturer data
    String expectedOutput;  // Expected classification
    bool shouldDetect;      // Should be classified as Lumos
};

TestPattern testCases[] = {
    {"4C00010201", "turn_left", true},
    {"4C00010202", "turn_right", true},
    {"4C00020101", "brake", true},
    {"4C00030101", "hazard", true},
    {"DEADBEEF", "unknown", false},
    {"", "no_data", false}
};
```

**Pass Criteria**:
- [ ] >95% accuracy on known patterns
- [ ] <5% false positive rate
- [ ] Graceful handling of malformed data
- [ ] Performance: <10ms per classification

## 🚴 Field Tests

### Test 6: Real-World BLE Environment

**Purpose**: Test system in realistic BLE environment
**Location**: Busy urban area with many BLE devices
**Duration**: 1 hour

**Procedure**:
1. Set up complete system on bicycle or bench
2. Record all BLE traffic for 1 hour
3. Analyze device detection patterns
4. Monitor system stability and performance

**Measurements**:
- Devices detected per minute
- Memory usage over time
- WiFi connection stability
- False positive rate

**Pass Criteria**:
- [ ] System stable for full test duration
- [ ] No memory leaks or crashes
- [ ] Reasonable device detection rate (>50 devices/hour)
- [ ] WiFi connection maintained throughout test

### Test 7: Range and Signal Strength Testing

**Purpose**: Determine effective range and signal thresholds
**Hardware**: Complete system + smartphone for distance measurement
**Duration**: 45 minutes

**Procedure**:
1. Place BLE sniffer at fixed location
2. Move test device (smartphone) at various distances
3. Record detection success rate vs distance
4. Test in different environments (open, urban, indoor)

**Distance Tests**:
- 1 meter (baseline)
- 5 meters
- 10 meters  
- 20 meters
- 50 meters
- 100 meters

**Pass Criteria**:
- [ ] 100% detection rate at 5 meters
- [ ] >90% detection rate at 10 meters
- [ ] >50% detection rate at 20 meters
- [ ] Graceful degradation with distance

### Test 8: Power Consumption Analysis

**Purpose**: Measure power consumption and battery life
**Hardware**: Current meters, battery packs
**Duration**: 24 hours (monitoring)

**Procedure**:
1. Measure idle current consumption
2. Measure active scanning current
3. Measure LED controller with various patterns
4. Calculate expected battery life

**Measurements**:
```
BLE Sniffer Unit:
- Idle: _____ mA
- Active BLE scanning: _____ mA
- WiFi active: _____ mA
- Peak consumption: _____ mA

LED Controller Unit:
- Idle: _____ mA
- Single LED strip active: _____ mA
- All LEDs active: _____ mA
- Peak consumption: _____ mA
```

**Pass Criteria**:
- [ ] BLE sniffer: <200mA average consumption
- [ ] LED controller: <1A average with normal use
- [ ] Battery life: >8 hours typical use
- [ ] Low battery warning functions correctly

## 🌧️ Environmental Tests

### Test 9: Weather Resistance

**Purpose**: Verify system operates in various weather conditions
**Duration**: Weather dependent

**Test Conditions**:
- Light rain (if enclosures used)
- Temperature range: 0°C to 40°C
- High humidity
- Vibration (simulate bicycle riding)

**Procedure**:
1. Seal system in weather-resistant enclosures
2. Expose to test conditions
3. Verify continued operation
4. Check for condensation or water ingress

**Pass Criteria**:
- [ ] No water ingress with proper enclosures
- [ ] Operation continues in temperature range
- [ ] No condensation on electronics
- [ ] System survives vibration testing

### Test 10: Interference Testing

**Purpose**: Test system performance in high-interference environments
**Location**: Near WiFi routers, microwaves, other 2.4GHz devices
**Duration**: 30 minutes

**Procedure**:
1. Establish baseline performance in clean environment
2. Introduce interference sources systematically
3. Monitor BLE detection rate and WiFi stability
4. Test recovery when interference removed

**Interference Sources**:
- Multiple WiFi networks
- Microwave oven
- Bluetooth speakers
- Other ESP32 devices

**Pass Criteria**:
- [ ] <20% degradation in BLE detection with interference
- [ ] WiFi connection maintained or reconnects quickly
- [ ] System recovers fully when interference removed
- [ ] No permanent damage or configuration loss

## 📊 Performance Benchmarks

### Baseline Performance Targets

| Metric | Target | Measurement Method |
|--------|--------|--------------------|
| BLE Scan Rate | >100 devices/min | Count unique devices in 1 minute |
| Pattern Recognition | <10ms latency | Time from BLE callback to classification |
| WiFi Response | <500ms | HTTP request response time |
| LED Latency | <100ms | Signal trigger to LED change |
| Memory Usage | <80% of available | Monitor free heap during operation |
| Battery Life (Sniffer) | >8 hours | Current measurement + calculation |
| Battery Life (LEDs) | >4 hours | With typical use patterns |

### Stress Testing

**High Load Test**:
- Run system continuously for 24 hours
- Generate high BLE traffic (multiple phones, tablets)
- Monitor for memory leaks or performance degradation

**Memory Stress Test**:
- Fill device lists to maximum capacity
- Fill signal buffers completely
- Verify graceful handling of memory limits

## 🐛 Failure Analysis

### Common Failure Modes

1. **BLE Scanning Stops**
   - Symptoms: No new devices detected
   - Debugging: Check ESP32 BLE stack status
   - Resolution: Restart BLE subsystem

2. **WiFi Connection Lost**
   - Symptoms: Web interface inaccessible
   - Debugging: Monitor WiFi status and signal strength
   - Resolution: Implement automatic reconnection

3. **LED Strip Malfunction**
   - Symptoms: Wrong colors, dead sections, flickering
   - Debugging: Test individual LEDs, check power supply
   - Resolution: Replace faulty components

4. **False Pattern Detection**
   - Symptoms: LEDs activate without real Lumos signals
   - Debugging: Analyze captured BLE data
   - Resolution: Refine pattern matching algorithms

### Debug Logging

Enable verbose logging for troubleshooting:

```cpp
#define DEBUG_VERBOSE 1

#ifdef DEBUG_VERBOSE
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_PRINTF(x, ...) Serial.printf(x, __VA_ARGS__)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(x, ...)
#endif
```

## ✅ Test Results Template

### Test Report Template

```markdown
# Test Report: [Test Name]
**Date**: YYYY-MM-DD
**Tester**: [Name]
**Hardware Version**: [Version]
**Software Version**: [Version]

## Test Results Summary
- **Pass**: [Number] tests
- **Fail**: [Number] tests  
- **Partial**: [Number] tests

## Detailed Results

### Test [Number]: [Name]
- **Status**: PASS/FAIL/PARTIAL
- **Duration**: [Time]
- **Notes**: [Observations]
- **Issues Found**: [List any problems]

### Performance Metrics
| Metric | Target | Measured | Pass/Fail |
|--------|--------|----------|-----------|
| [Metric] | [Value] | [Value] | [Status] |

## Recommendations
- [List of improvements or fixes needed]

## Next Steps
- [Actions to take based on results]
```

## 🚀 Automated Testing

### Continuous Integration Setup

For automated testing in development:

```yaml
# .github/workflows/test.yml
name: Hardware Tests
on: [push, pull_request]

jobs:
  compile:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v2
    - name: Setup Arduino CLI
      uses: arduino/setup-arduino-cli@v1
    - name: Install ESP32 platform
      run: |
        arduino-cli core update-index
        arduino-cli core install esp32:esp32
    - name: Compile sketches
      run: |
        arduino-cli compile --fqbn esp32:esp32:esp32 lumos_ble_sniffer/
        arduino-cli compile --fqbn esp32:esp32:esp32 trailer_led_controller/
```

### Hardware-in-the-Loop Testing

For automated hardware testing:

```cpp
// test_harness.ino - Automated test runner
void runAutomatedTests() {
    Serial.println("Starting automated test suite...");
    
    // Test 1: BLE Initialization
    bool bleTest = testBLEInit();
    reportResult("BLE Initialization", bleTest);
    
    // Test 2: WiFi Connectivity
    bool wifiTest = testWiFiConnection();
    reportResult("WiFi Connection", wifiTest);
    
    // Test 3: LED Functionality
    bool ledTest = testLEDStrips();
    reportResult("LED Strips", ledTest);
    
    // Generate test report
    generateTestReport();
}
```

---

This comprehensive testing guide ensures the Lumos BBR system is reliable, performant, and ready for real-world use. Regular testing during development helps catch issues early and validates that the system meets its design requirements.