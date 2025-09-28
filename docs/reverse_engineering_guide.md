# 🔬 Reverse Engineering Lumos BLE Protocols

This guide provides comprehensive instructions for analyzing and reverse engineering Bluetooth Low Energy (BLE) communications from Lumos Matrix smart bicycle helmets.

## 🎯 Overview

The Lumos Matrix helmet uses BLE to communicate with:
- Lumos mobile app
- Remote control units
- Other Lumos accessories

Our goal is to intercept and decode these communications to create an open-source relay system for bicycle trailers.

## 🛠️ Tools and Equipment

### Hardware Required
- **ESP32 development board** (primary sniffer)
- **nRF52840 USB dongle** (optional - professional analysis)
- **Lumos Matrix helmet** (target device)
- **Smartphone** with Lumos app
- **Logic analyzer** (optional - for deeper protocol analysis)

### Software Tools
- **Our BLE Sniffer** (this project)
- **nRF Connect** (Nordic mobile app)
- **Wireshark** with BLE plugin
- **nRF Sniffer** (professional tool)
- **Btlejuice** (BLE Man-in-the-Middle)

## 📡 BLE Fundamentals

### BLE Advertisement Structure
```
┌──────────────────────────────────────────────────────────┐
│                    BLE Advertisement                     │
├──────────────────────────────────────────────────────────┤
│ Preamble │ Access Address │ PDU Header │ Payload │ CRC   │
│ (1 byte) │   (4 bytes)    │ (2 bytes)  │(0-37 B) │(3 B)  │
└──────────────────────────────────────────────────────────┘

PDU Payload Structure:
┌─────────────────────────────────────────────────────────┐
│ AdvA (6B) │ AdvData (0-31 bytes)                       │
├───────────┼─────────────────────────────────────────────┤
│ MAC Addr  │ Type │ Length │ Data │ Type │ Length │ Data │
│           │ (1B) │  (1B)  │ (XB) │ (1B) │  (1B)  │ (YB) │
└─────────────────────────────────────────────────────────┘
```

### Advertisement Data Types (AD Types)
```cpp
// Common BLE Advertisement Data Types
#define AD_TYPE_FLAGS                    0x01
#define AD_TYPE_INCOMPLETE_16BIT_UUID    0x02
#define AD_TYPE_COMPLETE_16BIT_UUID      0x03
#define AD_TYPE_INCOMPLETE_32BIT_UUID    0x04
#define AD_TYPE_COMPLETE_32BIT_UUID      0x05
#define AD_TYPE_INCOMPLETE_128BIT_UUID   0x06
#define AD_TYPE_COMPLETE_128BIT_UUID     0x07
#define AD_TYPE_SHORT_LOCAL_NAME         0x08
#define AD_TYPE_COMPLETE_LOCAL_NAME      0x09
#define AD_TYPE_TX_POWER_LEVEL          0x0A
#define AD_TYPE_MANUFACTURER_DATA        0xFF
```

## 🔍 Analysis Methodology

### Phase 1: Passive Scanning

1. **Setup BLE Sniffer**:
   ```cpp
   // Configure for maximum sensitivity
   pBLEScan->setActiveScan(false);  // Passive scanning
   pBLEScan->setInterval(40);       // Fast scanning
   pBLEScan->setWindow(30);
   ```

2. **Capture Baseline Data**:
   - Record all BLE traffic for 5 minutes
   - Note background devices (phones, fitness trackers, etc.)
   - Create baseline device inventory

3. **Helmet Power-On Analysis**:
   - Power on Lumos helmet
   - Capture first 60 seconds of advertisements
   - Look for new devices in scan results

### Phase 2: Pattern Recognition

#### Device Identification
Look for these indicators of Lumos devices:

1. **Device Name Patterns**:
   ```cpp
   // Possible Lumos device names
   "Lumos"
   "Matrix" 
   "LUM_*"
   "LMTX_*"
   // Or completely generic names
   ```

2. **MAC Address Patterns**:
   ```cpp
   // Check for manufacturer-specific MAC ranges
   // Format: XX:XX:XX:YY:YY:YY
   // Where XX:XX:XX is manufacturer OUI
   // Common patterns:
   "A4:C1:38:*" // Texas Instruments
   "30:AE:A4:*" // Espressif
   "E4:5F:01:*" // Espressif
   ```

3. **Service UUID Patterns**:
   ```cpp
   // Standard Nordic UART Service
   "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
   
   // Custom Lumos services (to be discovered)
   "LUMOS-SERVICE-UUID-HERE"
   ```

#### Manufacturer Data Analysis

```cpp
void analyzeManufacturerData(const String& hexData) {
    // Example manufacturer data format:
    // "4C00" + payload (Apple format)
    // "FF00" + payload (Custom format)
    
    if (hexData.startsWith("4C00")) {
        // Apple-style manufacturer data
        analyzeAppleFormat(hexData.substring(4));
    } else if (hexData.startsWith("FF00")) {
        // Custom manufacturer format
        analyzeCustomFormat(hexData.substring(4));
    }
    
    // Look for patterns:
    // - Fixed headers
    // - Changing payload sections
    // - Checksums or CRC
}
```

### Phase 3: State Machine Analysis

#### Trigger-Response Mapping

1. **Button Press Analysis**:
   ```
   Action: Press left turn button
   Expected: BLE advertisement changes
   Capture: Before/after manufacturer data
   
   Action: Press right turn button  
   Expected: Different BLE pattern
   Capture: Compare with left turn data
   
   Action: Brake activation
   Expected: New advertisement type
   Capture: Brake-specific patterns
   ```

2. **App Interaction Analysis**:
   ```
   Action: Connect mobile app
   Expected: GATT connection establishment
   Capture: Service discovery process
   
   Action: Change settings in app
   Expected: GATT write operations
   Capture: Characteristic values
   ```

### Phase 4: Protocol Decoding

#### Signal Pattern Templates

Based on analysis, create templates for each signal type:

```cpp
struct LumosSignalPattern {
    String signalType;        // "turn_left", "turn_right", etc.
    String manufacturerData;  // Expected hex pattern
    String serviceUUID;       // GATT service if applicable
    int rssiThreshold;        // Minimum signal strength
    unsigned long duration;   // How long signal persists
};

// Example patterns (hypothetical)
LumosSignalPattern patterns[] = {
    {"turn_left",  "4C00010201", "", -70, 5000},
    {"turn_right", "4C00010202", "", -70, 5000}, 
    {"brake",      "4C00020101", "", -70, 3000},
    {"hazard",     "4C00030101", "", -70, 10000}
};
```

## 🧪 Practical Analysis Steps

### Step 1: Data Collection

```cpp
// Enhanced logging for reverse engineering
void logDetailedDevice(BLEAdvertisedDevice device) {
    Serial.println("=== DEVICE ANALYSIS ===");
    Serial.printf("Timestamp: %lu\n", millis());
    Serial.printf("Address: %s\n", device.getAddress().toString().c_str());
    Serial.printf("Address Type: %d\n", device.getAddressType());
    Serial.printf("Name: %s\n", device.getName().c_str());
    Serial.printf("RSSI: %d dBm\n", device.getRSSI());
    
    if (device.haveManufacturerData()) {
        std::string data = device.getManufacturerData();
        Serial.printf("Manufacturer Data (%d bytes): ", data.length());
        for (int i = 0; i < data.length(); i++) {
            Serial.printf("%02X ", (uint8_t)data[i]);
        }
        Serial.println();
    }
    
    if (device.haveServiceUUID()) {
        Serial.printf("Service UUID: %s\n", device.getServiceUUID().toString().c_str());
    }
    
    Serial.println("========================");
}
```

### Step 2: Pattern Detection

```cpp
// Automatic pattern detection
class PatternDetector {
private:
    std::vector<String> knownPatterns;
    std::map<String, int> patternCounts;
    
public:
    void addObservation(const String& data) {
        patternCounts[data]++;
        
        if (patternCounts[data] > 3) {
            // Pattern seen multiple times, likely significant
            analyzePattern(data);
        }
    }
    
    void analyzePattern(const String& data) {
        // Statistical analysis of pattern
        // - Check for fixed vs variable sections
        // - Look for incremental counters
        // - Identify potential checksums
    }
};
```

### Step 3: Validation Testing

```cpp
// Test pattern recognition accuracy
void validatePattern(const String& expectedType, const String& testData) {
    String detectedType = classifySignalPattern(testData, -60);
    
    if (detectedType == expectedType) {
        Serial.printf("✓ Pattern validation PASSED: %s\n", expectedType.c_str());
    } else {
        Serial.printf("✗ Pattern validation FAILED: Expected %s, got %s\n", 
                     expectedType.c_str(), detectedType.c_str());
    }
}
```

## 📊 Data Analysis Techniques

### Statistical Analysis

1. **Frequency Analysis**:
   ```cpp
   // Count byte frequency in manufacturer data
   void analyzeByteFrequency(const std::vector<String>& samples) {
       std::map<uint8_t, int> frequency;
       
       for (const String& sample : samples) {
           for (int i = 0; i < sample.length(); i += 3) {
               String hex = sample.substring(i, i + 2);
               uint8_t byte = strtol(hex.c_str(), NULL, 16);
               frequency[byte]++;
           }
       }
       
       // Print frequency distribution
       for (auto& pair : frequency) {
           Serial.printf("Byte 0x%02X: %d occurrences\n", pair.first, pair.second);
       }
   }
   ```

2. **Entropy Analysis**:
   ```cpp
   // Calculate Shannon entropy of data
   double calculateEntropy(const String& data) {
       std::map<char, int> frequency;
       for (char c : data) frequency[c]++;
       
       double entropy = 0.0;
       int length = data.length();
       
       for (auto& pair : frequency) {
           double probability = (double)pair.second / length;
           entropy -= probability * log2(probability);
       }
       
       return entropy;
   }
   ```

### Temporal Analysis

```cpp
// Analyze timing patterns
struct TimingAnalysis {
    unsigned long firstSeen;
    unsigned long lastSeen;
    std::vector<unsigned long> intervals;
    
    void addTimestamp(unsigned long timestamp) {
        if (firstSeen == 0) {
            firstSeen = timestamp;
        } else {
            intervals.push_back(timestamp - lastSeen);
        }
        lastSeen = timestamp;
    }
    
    void printStats() {
        if (intervals.empty()) return;
        
        unsigned long total = 0;
        unsigned long min_interval = intervals[0];
        unsigned long max_interval = intervals[0];
        
        for (unsigned long interval : intervals) {
            total += interval;
            if (interval < min_interval) min_interval = interval;
            if (interval > max_interval) max_interval = interval;
        }
        
        unsigned long average = total / intervals.size();
        
        Serial.printf("Timing Analysis:\n");
        Serial.printf("  Average interval: %lu ms\n", average);
        Serial.printf("  Min interval: %lu ms\n", min_interval);
        Serial.printf("  Max interval: %lu ms\n", max_interval);
        Serial.printf("  Total observations: %d\n", intervals.size());
    }
};
```

## 🔐 Security Considerations

### Encryption Detection

```cpp
// Check if data appears encrypted
bool isDataEncrypted(const String& data) {
    // Simple entropy check
    double entropy = calculateEntropy(data);
    
    // High entropy (>7.0) may indicate encryption
    if (entropy > 7.0) {
        Serial.printf("High entropy detected (%.2f) - possible encryption\n", entropy);
        return true;
    }
    
    // Look for common patterns that indicate plaintext
    if (data.indexOf("00000000") >= 0 || 
        data.indexOf("FFFFFFFF") >= 0 ||
        data.indexOf("12345678") >= 0) {
        return false;  // Likely not encrypted
    }
    
    return false;  // Assume not encrypted
}
```

### Authentication Analysis

```cpp
// Look for authentication/pairing sequences
void analyzeAuthentication(const String& deviceAddress, const String& data) {
    static std::map<String, std::vector<String>> deviceSequences;
    
    deviceSequences[deviceAddress].push_back(data);
    
    // Check for challenge-response patterns
    if (deviceSequences[deviceAddress].size() >= 3) {
        // Analyze last 3 messages for patterns
        analyzeSequencePattern(deviceSequences[deviceAddress]);
    }
}
```

## 📈 Advanced Analysis Tools

### Machine Learning Approach

```cpp
// Simple pattern classifier using similarity matching
class PatternClassifier {
private:
    struct Pattern {
        String type;
        String template_data;
        double confidence;
    };
    
    std::vector<Pattern> knownPatterns;
    
public:
    void trainPattern(const String& type, const String& data) {
        Pattern p = {type, data, 1.0};
        knownPatterns.push_back(p);
    }
    
    String classify(const String& input) {
        double bestMatch = 0.0;
        String bestType = "unknown";
        
        for (const Pattern& p : knownPatterns) {
            double similarity = calculateSimilarity(input, p.template_data);
            if (similarity > bestMatch) {
                bestMatch = similarity;
                bestType = p.type;
            }
        }
        
        return bestMatch > 0.7 ? bestType : "unknown";
    }
    
private:
    double calculateSimilarity(const String& a, const String& b) {
        // Levenshtein distance-based similarity
        // Implementation details...
        return 0.0;  // Placeholder
    }
};
```

## 🎯 Target Patterns to Discover

Based on Lumos helmet functionality, look for these signal types:

### Turn Signals
- **Left Turn**: Likely 500ms blink pattern
- **Right Turn**: Different from left turn pattern
- **Auto-cancel**: Signal stops after timeout or straight motion

### Brake Signals
- **Brake Activation**: Triggered by deceleration
- **Brake Release**: Signal ends when speed stabilizes

### Status Signals
- **Battery Level**: Periodic status broadcasts
- **Connection Status**: Pairing/unpairing events
- **Settings Changes**: Configuration updates

## 📝 Documentation Template

For each discovered pattern, document using this template:

```markdown
## Pattern: [SIGNAL_TYPE]

**Discovery Date**: YYYY-MM-DD
**Confidence Level**: High/Medium/Low
**Validation Status**: Tested/Theoretical

### Signal Characteristics
- **Trigger**: [What causes this signal]
- **Duration**: [How long signal lasts]
- **Frequency**: [How often it repeats]
- **RSSI Range**: [Signal strength range]

### BLE Advertisement Data
```
Manufacturer Data: [HEX_STRING]
Service UUID: [UUID_IF_APPLICABLE]
Device Name: [NAME_IF_CHANGED]
```

### Validation Tests
- [ ] Manual trigger test
- [ ] Timing verification
- [ ] Range testing
- [ ] Interference testing

### Implementation
```cpp
// Pattern detection code
if (manufacturerData == "[PATTERN]") {
    return "[SIGNAL_TYPE]";
}
```
```

## 🚨 Legal and Ethical Considerations

### Compliance Guidelines

1. **Passive Analysis Only**: Only intercept broadcast advertisements, never attempt to connect or interfere
2. **Personal Use**: Reverse engineering for personal use and interoperability is generally legal
3. **No Trademark Infringement**: Don't use Lumos trademarks or logos
4. **Safety First**: Ensure any implementations don't compromise bicycle safety
5. **Responsible Disclosure**: Report security vulnerabilities to Lumos appropriately

### Best Practices

- Document all findings for reproducibility
- Share discoveries with the open-source community
- Respect intellectual property rights
- Focus on interoperability, not competition
- Maintain ethical hacking principles

## 📚 Resources and References

### Technical Documentation
- [Bluetooth Core Specification](https://www.bluetooth.com/specifications/bluetooth-core-specification/)
- [BLE Advertisement Data Format](https://www.bluetooth.com/specifications/assigned-numbers/generic-access-profile/)
- [ESP32 BLE Arduino Documentation](https://github.com/nkolban/ESP32_BLE_Arduino)

### Analysis Tools
- [nRF Connect for Mobile](https://www.nordicsemi.com/Software-and-tools/Development-Tools/nRF-Connect-for-mobile)
- [Wireshark BLE Analysis](https://wiki.wireshark.org/Bluetooth)
- [BLE Scanner Apps](https://play.google.com/store/apps/details?id=com.macdom.ble.blescanner)

### Community Resources
- [Bluetooth LE Security](https://lacklustre.net/bluetooth/)
- [BLE Reverse Engineering Guide](https://github.com/homewsn/BLEDissect)
- [IoT Security Research](https://iotsecuritywiki.com/)

---

**Remember**: This guide is for educational and research purposes. Always follow local laws and ethical guidelines when conducting reverse engineering activities.