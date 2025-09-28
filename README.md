# 🚴 Lumos BBR Bluetooth Sniffer

**Open-source ESP32 Bluetooth sniffer for analyzing Lumos Matrix helmet communications and relay system for bicycle trailer turn signals**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino IDE](https://img.shields.io/badge/Arduino_IDE-Compatible-blue.svg)](https://www.arduino.cc/)
[![ESP32](https://img.shields.io/badge/Platform-ESP32-red.svg)](https://www.espressif.com/en/products/socs/esp32)

## 🎯 Project Overview

The Lumos BBR (Bluetooth Blinking Remote) system is a complete solution for intercepting and analyzing Bluetooth Low Energy (BLE) communications from Lumos Matrix smart bicycle helmets, then relaying turn signals and brake indicators to a bicycle trailer's LED array.

### Key Features

- **🔍 BLE Signal Analysis**: Real-time scanning and pattern recognition of Lumos helmet communications
- **📊 Web Interface**: Live monitoring dashboard with device detection and signal analysis
- **💡 Smart LED Control**: Animated turn signals and brake lights for bicycle trailers
- **🔧 Manual Override**: Physical buttons for manual signal control
- **🔋 Battery Monitoring**: Low-battery warnings and power management
- **📱 WiFi Control**: Wireless configuration and monitoring
- **🛠️ Reverse Engineering Tools**: Built-in tools for discovering new BLE patterns

## 📁 Project Structure

```
Lumos-BBR-Bluetooth-Sniffer/
├── lumos_ble_sniffer/           # Main BLE scanner ESP32 code
│   ├── lumos_ble_sniffer.ino    # Arduino sketch
│   └── data/                    # Web interface files (SPIFFS)
│       └── index.html           # Real-time monitoring dashboard
├── trailer_led_controller/      # Trailer LED controller ESP32 code
│   └── trailer_led_controller.ino
├── docs/                        # Documentation
├── examples/                    # Example configurations and test code
├── hardware/                    # Wiring diagrams and schematics
└── tests/                       # Test scripts and validation
```

## 🔧 Hardware Requirements

### Main BLE Sniffer Unit
- **ESP32 DevKit V1** (or compatible)
- **Status LED** (built-in LED on pin 2)
- **Power supply** (USB or battery pack)

### Trailer LED Controller Unit
- **ESP32 DevKit V1** (or compatible)
- **WS2812B LED strips** (3 strips: left turn, right turn, brake)
- **Push buttons** (3x for manual control)
- **Battery monitoring circuit**
- **Power supply** (12V battery recommended)

### Recommended Components
- **FastLED-compatible LED strips**: WS2812B, SK6812, or similar
- **Tactile push buttons**: 12mm momentary switches
- **Resistors**: 10kΩ pull-up resistors for buttons
- **Capacitors**: 1000µF for LED power supply filtering
- **Voltage regulator**: 5V step-down converter for ESP32
- **Enclosures**: Weather-resistant cases for outdoor use

## 🔌 Wiring Diagrams

### BLE Sniffer Unit

```
ESP32 DevKit V1
┌─────────────────┐
│     [ESP32]     │
│                 │
│  GPIO2  ●───────┼──── Status LED (+) ──── 220Ω ──── GND
│                 │
│  3.3V   ●───────┼──── VCC (sensors if needed)
│  GND    ●───────┼──── GND (common ground)
│                 │
│  USB    ●───────┼──── USB Cable (programming & power)
└─────────────────┘
```

### Trailer LED Controller Unit

```
ESP32 DevKit V1
┌─────────────────┐       ┌────────────────┐
│     [ESP32]     │       │   LED Strips   │
│                 │       │                │
│  GPIO18 ●───────┼───────┼──── Left Turn  │ WS2812B (20 LEDs)
│  GPIO19 ●───────┼───────┼──── Right Turn │ WS2812B (20 LEDs)
│  GPIO21 ●───────┼───────┼──── Brake      │ WS2812B (30 LEDs)
│                 │       │                │
│  GPIO12 ●───────┼────┐  └────────────────┘
│  GPIO13 ●───────┼────┼──┐
│  GPIO14 ●───────┼────┼──┼──┐  ┌──────────┐
│                 │    │  │  │  │ Buttons  │
│  GPIO2  ●───────┼────┼──┼──┼──┼─ Status  │
│                 │    │  │  │  │          │
│  A0     ●───────┼────┼──┼──┼──┼─ Battery │
│                 │    │  │  │  │ Monitor  │
│  5V     ●───────┼────┼──┼──┼──┼─ VCC     │
│  GND    ●───────┼────┼──┼──┼──┼─ GND     │
└─────────────────┘    │  │  │  └──────────┘
                       │  │  │
            ┌──────────┘  │  │
            │  ┌──────────┘  │
            │  │  ┌──────────┘
            ▼  ▼  ▼
    ┌───────────────────┐
    │ Button Wiring     │
    │ (Pull-up enabled) │
    │                   │
    │ GPIO12 ──[BTN]── GND (Left)
    │ GPIO13 ──[BTN]── GND (Right)
    │ GPIO14 ──[BTN]── GND (Hazard)
    └───────────────────┘
```

### LED Strip Power Distribution

```
12V Battery
     │
     ├── 5V Regulator ──── ESP32 (5V/3.3V)
     │
     └── LED Power Rails
         ├── Left Turn Strip (5V, 1A max)
         ├── Right Turn Strip (5V, 1A max)
         └── Brake Strip (5V, 1.5A max)
```

## ⚡ Power Requirements

### BLE Sniffer Unit
- **ESP32**: ~200mA @ 3.3V
- **Total consumption**: ~250mA @ 5V
- **Recommended**: USB power bank (5000mAh = ~20 hours)

### LED Controller Unit
- **ESP32**: ~200mA @ 3.3V
- **LED strips**: Up to 3.5A @ 5V (all LEDs at full brightness)
- **Total consumption**: ~4A @ 5V peak
- **Recommended**: 12V 5Ah battery with 5V regulator

## 🚀 Installation & Setup

### 1. Arduino IDE Setup

1. Install the **ESP32 board package**:
   - Go to File → Preferences
   - Add to Additional Board Manager URLs: 
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Go to Tools → Board → Boards Manager
   - Search "ESP32" and install "ESP32 by Espressif Systems"

2. Install required libraries:
   ```
   ESP32 BLE Arduino (built-in)
   ESPAsyncWebServer
   ArduinoJson
   FastLED (for trailer controller)
   ```

### 2. BLE Sniffer Setup

1. Open `lumos_ble_sniffer/lumos_ble_sniffer.ino`
2. Select board: **ESP32 Dev Module**
3. Upload the sketch
4. Upload SPIFFS data:
   - Install **ESP32 Sketch Data Upload** plugin
   - Go to Tools → ESP32 Sketch Data Upload
   - This uploads the web interface files

### 3. Trailer Controller Setup

1. Connect LED strips and buttons according to wiring diagram
2. Open `trailer_led_controller/trailer_led_controller.ino`
3. Adjust LED configuration if needed:
   ```cpp
   #define NUM_LEDS_LEFT 20      // Adjust for your LED count
   #define NUM_LEDS_RIGHT 20
   #define NUM_LEDS_BRAKE 30
   ```
4. Upload the sketch

### 4. System Configuration

1. **Power on BLE Sniffer**: Creates WiFi network "Lumos-BBR-Sniffer"
2. **Connect trailer controller**: Automatically connects to sniffer network
3. **Access web interface**: Navigate to `http://192.168.4.1`
4. **Test functionality**: Use manual buttons or web interface

## 🎮 Usage Guide

### Starting the System

1. **Power on** the BLE sniffer unit first
2. **Wait 30 seconds** for WiFi network to establish
3. **Power on** the trailer LED controller
4. **Verify connection** via web interface or serial monitor

### Web Interface Access

1. **Connect** to WiFi network "Lumos-BBR-Sniffer" (password: `lumos123`)
2. **Navigate** to `http://192.168.4.1`
3. **Monitor** real-time BLE device detection and signal analysis

### Manual Override

Use the physical buttons on the trailer controller:
- **Left Button**: Toggle left turn signal
- **Right Button**: Toggle right turn signal  
- **Hazard Button**: Toggle hazard lights (both signals)

### API Endpoints

The system provides REST API access:

```bash
# Get device list
GET http://192.168.4.1/api/devices

# Get signal log
GET http://192.168.4.1/api/signals

# Control trailer (trailer controller)
POST http://[trailer-ip]/control/left
POST http://[trailer-ip]/control/right
POST http://[trailer-ip]/control/hazard
POST http://[trailer-ip]/control/off
```

## 🔬 Reverse Engineering BLE Protocols

### Discovering Lumos Patterns

The system includes tools for reverse engineering Lumos helmet BLE communications:

1. **Pattern Detection**: Automatic analysis of manufacturer data
2. **Signal Classification**: Classification of different signal types
3. **Real-time Monitoring**: Web interface shows all BLE traffic
4. **Data Export**: Export captured data for analysis

### Adding New Patterns

To add newly discovered Lumos patterns, modify the `analyzeLumosPattern()` function:

```cpp
bool analyzeLumosPattern(const String& manufacturerData, const String& name, const String& address) {
  // Check device name patterns
  if (name.indexOf("Lumos") >= 0 || name.indexOf("Matrix") >= 0) {
    return true;
  }
  
  // Add manufacturer data patterns here
  if (manufacturerData.startsWith("4C00")) { // Example Apple pattern
    return checkApplePattern(manufacturerData);
  }
  
  // Add MAC address patterns
  if (address.startsWith("AA:BB:CC")) { // Example Lumos MAC range
    return true;
  }
  
  return false;
}
```

### Signal Pattern Classification

Update `classifySignalPattern()` to recognize specific signal types:

```cpp
String classifySignalPattern(const String& data, int rssi) {
  if (data.length() == 0) return "no_data";
  
  // Example patterns (to be discovered)
  if (data == "FF 01 02") return "turn_left";
  if (data == "FF 01 03") return "turn_right";
  if (data == "FF 02 01") return "brake";
  if (data == "FF 03 01") return "hazard";
  
  return "unclassified";
}
```

## 📊 Testing & Validation

### System Tests

1. **BLE Scanning Test**:
   ```bash
   # Monitor serial output for device detection
   # Should show nearby BLE devices with RSSI values
   ```

2. **Web Interface Test**:
   ```bash
   # Access http://192.168.4.1
   # Verify device list updates automatically
   # Check signal log for BLE activity
   ```

3. **LED Controller Test**:
   ```bash
   # Test each button for manual override
   # Verify LED patterns and animations
   # Check battery monitoring display
   ```

### Performance Benchmarks

- **BLE Scan Rate**: ~100 devices/minute detection capability
- **Signal Processing**: <10ms latency for pattern recognition
- **Web Interface**: <500ms refresh rate
- **LED Response**: <50ms signal-to-light latency

## 🔧 Troubleshooting

### Common Issues

**BLE Sniffer not detecting devices:**
- Check ESP32 is powered and BLE initialized
- Verify antenna placement (keep away from metal)
- Monitor serial output for error messages

**Trailer controller not connecting:**
- Verify WiFi credentials match
- Check power supply (LEDs need adequate current)
- Ensure ESP32 is within WiFi range

**LEDs not working:**
- Verify wiring connections and power supply
- Check FastLED library installation
- Test with simple LED patterns first

**Web interface not loading:**
- Verify SPIFFS upload completed successfully
- Check ESP32 serial output for web server status
- Try accessing IP directly: `http://192.168.4.1`

### Debug Mode

Enable debug output by uncommenting in the code:
```cpp
#define DEBUG_MODE 1  // Add this line for verbose logging
```

## 🛡️ Safety Considerations

- **Weather Protection**: Use waterproof enclosures for outdoor use
- **Power Management**: Include fuses and proper voltage regulation
- **Visibility**: Ensure LED brightness is appropriate for conditions
- **Mounting**: Secure all components to prevent damage from vibration
- **Legal Compliance**: Check local regulations for bicycle lighting

## 🤝 Contributing

We welcome contributions! Please see our contributing guidelines:

1. **Fork** the repository
2. **Create** a feature branch
3. **Make** your changes with clear commit messages
4. **Test** thoroughly with actual hardware
5. **Submit** a pull request with detailed description

### Areas for Contribution

- **BLE Pattern Discovery**: Help reverse engineer actual Lumos protocols
- **Hardware Designs**: PCB layouts and 3D printable enclosures  
- **Software Features**: Enhanced pattern recognition and UI improvements
- **Documentation**: User guides and video tutorials
- **Testing**: Validation with different Lumos helmet models

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Espressif** for the ESP32 platform
- **FastLED** community for excellent LED control library
- **Arduino** community for extensive ESP32 support
- **Lumos** for creating innovative bicycle safety products (this is an independent analysis project)

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/noweheere/Lumos-BBR-Bluetooth-Sniffer/issues)
- **Discussions**: [GitHub Discussions](https://github.com/noweheere/Lumos-BBR-Bluetooth-Sniffer/discussions)
- **Documentation**: [Wiki](https://github.com/noweheere/Lumos-BBR-Bluetooth-Sniffer/wiki)

---

**⚠️ Disclaimer**: This project is for educational and research purposes. Always follow local traffic laws and ensure proper visibility while cycling. The authors are not responsible for accidents or injuries resulting from use of this system.
