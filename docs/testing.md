# Testing Guide

This guide provides comprehensive testing procedures for the Lumos BBR Bluetooth Sniffer system to ensure proper functionality and safety.

## Pre-Testing Checklist

### Hardware Verification
- [ ] All connections secure and properly insulated
- [ ] Correct power supply voltages (12V, 5V)
- [ ] Fuses installed and rated correctly
- [ ] ESP32 boards programmed with latest firmware
- [ ] LED indicators and outputs connected properly

### Software Verification
- [ ] Both ESP32s boot successfully
- [ ] Serial communication working at 115200 baud
- [ ] Configuration files updated with correct settings
- [ ] MAC addresses configured correctly
- [ ] Libraries installed and up-to-date

## Phase 1: Individual Component Testing

### Test 1: ESP32 Sniffer Basic Function

**Objective**: Verify sniffer ESP32 boots and scans for BLE devices

**Procedure**:
1. Connect sniffer ESP32 to computer via USB
2. Open Serial Monitor at 115200 baud
3. Reset ESP32 and observe boot sequence

**Expected Output**:
```
Lumos BBR Bluetooth Sniffer Starting...
BLE Scanner initialized
Scanning for Lumos devices...
Scan completed. Found X devices.
=== STATUS ===
Device found: NO
Left indicator: OFF
Right indicator: OFF
Hazard lights: OFF
Signal history: 0 entries
Free heap: XXXXX bytes
=============
```

**Pass Criteria**: Clean boot, no error messages, BLE scanning active

### Test 2: ESP32 Trailer Basic Function

**Objective**: Verify trailer ESP32 boots and responds to commands

**Procedure**:
1. Connect trailer ESP32 to computer via USB
2. Open Serial Monitor at 115200 baud  
3. Reset ESP32 and observe boot sequence
4. Send command `status` via Serial Monitor

**Expected Output**:
```
Lumos BBR Trailer Relay System Starting...
Trailer relay system initialized
Waiting for commands from sniffer...
=== TRAILER RELAY STATUS ===
Connection active: NO
Left indicator: OFF
Right indicator: OFF
Hazard lights: OFF
Brake lights: OFF
Last command time: 0 ms ago
Free heap: XXXXX bytes
============================
```

**Pass Criteria**: Clean boot, responds to status command, all outputs OFF

### Test 3: Hardware Output Testing

**Objective**: Verify all output channels function correctly

**Procedure**:
1. Connect trailer ESP32 with actual LED loads
2. Send test command `test` via Serial Monitor
3. Observe LED operation sequence

**Expected Behavior**:
```
Testing Sequence:
1. Left indicator ON for 1 second
2. Right indicator ON for 1 second  
3. Brake lights ON for 1 second
4. All lights ON for 1 second
5. All lights OFF
```

**Pass Criteria**: All LEDs activate in correct sequence, proper brightness, no flickering

## Phase 2: Communication Testing

### Test 4: ESP-NOW Communication

**Objective**: Verify wireless communication between ESP32 units

**Procedure**:
1. Power both ESP32s independently
2. Monitor both serial outputs
3. Send `left_on` command to trailer ESP32
4. Verify sniffer ESP32 can send commands to trailer

**Setup for ESP-NOW Test**:
```cpp
// Add this test code to verify ESP-NOW is working
void testESPNOW() {
    CommandData testCmd = {1, true, millis(), "test"};
    esp_now_send(trailerMacAddress, (uint8_t*)&testCmd, sizeof(testCmd));
    Serial.println("Test command sent via ESP-NOW");
}
```

**Pass Criteria**: Commands transmitted and received successfully, minimal latency (<100ms)

### Test 5: Serial Communication Fallback

**Objective**: Test backup communication method

**Procedure**:
1. Connect ESP32s with serial cable (TX->RX, RX->TX, GND->GND)
2. Disable ESP-NOW in configuration
3. Test command transmission via serial

**Pass Criteria**: Commands transmitted via serial when ESP-NOW unavailable

## Phase 3: BLE Signal Analysis Testing

### Test 6: Generic BLE Device Detection

**Objective**: Verify BLE scanning functionality with known devices

**Procedure**:
1. Enable smartphone Bluetooth with discoverable mode
2. Place phone near sniffer ESP32
3. Monitor serial output for device detection

**Expected Output**:
```
Found BLE device: [Phone Name]
Address: XX:XX:XX:XX:XX:XX
RSSI: -XX dBm
```

**Pass Criteria**: Detects and logs BLE devices with correct information

### Test 7: Lumos Device Simulation

**Objective**: Test pattern recognition with simulated Lumos signals

**Setup**: Create test BLE advertiser (using second ESP32 or phone app)

**Procedure**:
1. Configure test device to advertise with Lumos-like patterns
2. Observe sniffer response to different signal patterns
3. Verify correct signal type detection

**Test Patterns**:
- Left turn: `{0x01, 0x00, 0x01, 0x00}`
- Right turn: `{0x02, 0x00, 0x02, 0x00}`  
- Hazard: `{0x03, 0x00, 0x03, 0x00}`
- Brake: `{0x00, 0x00, 0x00, 0x80}`

**Pass Criteria**: Correct signal type identification and relay commands

## Phase 4: Integrated System Testing

### Test 8: End-to-End Signal Chain

**Objective**: Test complete signal chain from BLE to trailer lights

**Procedure**:
1. Set up complete system with both ESP32s and trailer lights
2. Use BLE signal simulator or actual Lumos helmet
3. Verify correct light activation for each signal type

**Test Scenarios**:
- Left turn signal → Left trailer indicator flashing
- Right turn signal → Right trailer indicator flashing
- Hazard signal → Both indicators flashing (faster rate)
- Brake signal → Brake lights solid ON

**Pass Criteria**: Correct light patterns, proper timing, no false triggers

### Test 9: Timing and Safety Testing

**Objective**: Verify safety timeouts and failsafe behavior

**Procedure**:
1. Activate indicators with simulated signals
2. Stop signal transmission
3. Verify automatic shutoff after timeout period

**Expected Behavior**:
- Indicators turn OFF after 3 seconds of no signal
- Status LED indicates lost connection
- Emergency stop function works immediately

**Pass Criteria**: All safety features function as designed

## Phase 5: Performance and Reliability Testing

### Test 10: Range Testing

**Objective**: Determine effective communication range

**Procedure**:
1. Place sniffer ESP32 at various distances from signal source
2. Test trailer ESP32 communication range
3. Document signal strength vs. distance

**Test Distances**:
- 1 meter, 5 meters, 10 meters, 20 meters, 50 meters

**Pass Criteria**: Reliable operation at intended deployment distances

### Test 11: Power Consumption Testing

**Objective**: Measure system power consumption

**Equipment**: Digital multimeter with current measurement

**Procedure**:
1. Measure current draw in different operating states
2. Test battery life projections
3. Verify power management features

**Measurement Points**:
- Idle state (scanning only)
- Active communication
- All lights ON
- Sleep mode (if implemented)

**Pass Criteria**: Power consumption within design specifications

### Test 12: Environmental Testing

**Objective**: Test system reliability under various conditions

**Test Conditions**:
- Temperature: -10°C to +50°C
- Humidity: 10% to 90% RH
- Vibration: Typical vehicle/trailer conditions
- EMI: Test near radio transmitters

**Procedure**:
1. Operate system in each test condition
2. Monitor for errors or performance degradation
3. Verify continued operation after environmental stress

**Pass Criteria**: Stable operation in all expected conditions

## Phase 6: Real-World Testing

### Test 13: Actual Lumos Helmet Testing

**Objective**: Test with real Lumos Matrix helmet (if available)

**Procedure**:
1. Pair system with actual Lumos helmet
2. Test all helmet functions (turn signals, brake, hazard)
3. Verify signal pattern recognition accuracy
4. Test at various distances and orientations

**Pass Criteria**: 100% signal recognition accuracy, stable operation

### Test 14: Vehicle Integration Testing

**Objective**: Test in actual vehicle/trailer environment

**Procedure**:
1. Install system in test vehicle with trailer
2. Test during actual driving conditions
3. Verify operation with vehicle electrical system
4. Test interference with other vehicle electronics

**Pass Criteria**: No interference, reliable operation, proper light visibility

## Troubleshooting Common Test Failures

### BLE Scanning Issues
**Symptoms**: No devices detected, scan errors
**Solutions**:
- Check ESP32 board package version
- Verify antenna connections
- Try different ESP32 board
- Check for interference sources

### Communication Failures
**Symptoms**: Commands not received, timeout errors
**Solutions**:  
- Verify MAC addresses in configuration
- Check power supply stability
- Test with shorter distances
- Monitor for interference

### Light Control Issues
**Symptoms**: Lights not activating, wrong patterns
**Solutions**:
- Check MOSFET gate voltages
- Verify current limiting resistors
- Test power supply under load
- Check GPIO pin assignments

### Performance Issues
**Symptoms**: Slow response, missed signals, crashes
**Solutions**:
- Monitor memory usage
- Check task priorities
- Verify timing configurations
- Add watchdog timers

## Test Documentation

### Test Results Template

```
TEST: [Test Name]
DATE: [Date]
TESTER: [Name]
HARDWARE: [ESP32 versions, board revisions]
SOFTWARE: [Firmware versions]

PROCEDURE:
[Step-by-step test procedure]

RESULTS:
[Actual results observed]

PASS/FAIL: [Status]
NOTES: [Additional observations]
```

### Performance Metrics

Track these metrics during testing:

- **BLE Detection Rate**: Percentage of signals correctly detected
- **Communication Latency**: Time from BLE signal to light activation
- **Range**: Maximum reliable communication distance
- **Battery Life**: Operating time on battery power
- **MTBF**: Mean time between failures
- **False Positive Rate**: Incorrect signal detections

## Continuous Testing

### Automated Test Scripts

Create automated tests for regression testing:

```python
# Example automated test script
import serial
import time
import json

def test_basic_functionality():
    ser = serial.Serial('/dev/ttyUSB0', 115200)
    
    # Send test commands
    commands = ['status', 'test', 'left_on', 'all_off']
    
    for cmd in commands:
        ser.write(cmd.encode() + b'\n')
        time.sleep(1)
        response = ser.readline().decode()
        print(f"Command: {cmd}, Response: {response}")
```

### Long-Term Reliability Testing

- **Burn-in Testing**: 48-hour continuous operation
- **Cycle Testing**: Repeated on/off cycles
- **Stress Testing**: Maximum load conditions
- **Temperature Cycling**: Extended temperature variation

## Test Schedule

### Development Phase
- Daily: Basic functionality tests
- Weekly: Full integration testing
- Before release: Complete test suite

### Production Phase  
- Pre-installation: Hardware verification
- Post-installation: System validation
- Periodic: Performance monitoring

## Pass/Fail Criteria Summary

### Critical (Must Pass)
- [ ] System boots without errors
- [ ] BLE scanning functional
- [ ] Inter-ESP32 communication working
- [ ] All light outputs functional
- [ ] Safety timeouts working
- [ ] Emergency stop functional

### Important (Should Pass)
- [ ] Signal pattern recognition accurate
- [ ] Communication range adequate
- [ ] Power consumption acceptable
- [ ] Environmental tolerance good

### Optional (Nice to Have)
- [ ] Web interface functional
- [ ] Advanced features working
- [ ] Extended range operation
- [ ] Low-power modes working

## Next Steps After Testing

1. Document all test results
2. Address any failed tests
3. Update configuration as needed
4. Prepare for deployment
5. Create user training materials
6. Establish maintenance procedures

For support with testing issues, refer to the [Troubleshooting section](installation.md#troubleshooting) or open a GitHub issue with detailed test results.