# Installation Guide

This guide provides step-by-step instructions for setting up the Lumos BBR Bluetooth Sniffer system.

## Prerequisites

### Hardware Requirements

- **2x ESP32 Development Boards**
  - ESP32-WROOM-32 or ESP32-WROVER-32 recommended
  - Minimum 4MB Flash memory
  - Built-in WiFi and Bluetooth required

- **Power Supply**
  - 12V DC supply for trailer system (minimum 5A capacity)
  - USB power for ESP32 boards (during development)
  - Optional: 12V to 5V converter for ESP32 power in vehicle

- **Interface Components**
  - Relay modules (5V coil, 12V/10A contacts) or MOSFET drivers
  - LED indicators or light strips
  - Breadboard or custom PCB
  - Jumper wires and connectors

### Software Requirements

- **Arduino IDE 2.0+** or **PlatformIO**
- **ESP32 Board Package** (version 2.0.0+)
- **USB-to-Serial Drivers** for ESP32 programming

## Step 1: Arduino IDE Setup

### Install Arduino IDE
1. Download Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software)
2. Install and launch Arduino IDE

### Add ESP32 Board Support
1. Open **File > Preferences**
2. Add ESP32 board URL to "Additional Boards Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Go to **Tools > Board > Boards Manager**
4. Search for "ESP32" and install **ESP32 by Espressif Systems**
5. Select **ESP32 Dev Module** as board type

## Step 2: Install Required Libraries

### Core Libraries (install via Library Manager)

1. Open **Sketch > Include Library > Manage Libraries**
2. Install the following libraries:

```
ArduinoJson (version 6.19.0+)
ESP32Time (version 2.0.0+)
AsyncTCP (version 1.1.1+)
ESPAsyncWebServer (version 1.2.3+)
```

### Manual Library Installation

Some libraries may need manual installation:

1. Download library ZIP files
2. Use **Sketch > Include Library > Add .ZIP Library**
3. Select downloaded ZIP file

## Step 3: Download and Configure Project

### Clone Repository
```bash
git clone https://github.com/noweheere/Lumos-BBR-Bluetooth-Sniffer.git
cd Lumos-BBR-Bluetooth-Sniffer
```

### Configure Hardware Settings

1. Edit `config/bluetooth_sniffer_config.h`:
   ```cpp
   #define SNIFFER_LED_PIN 2          // Your LED pin
   #define SNIFFER_RELAY_PIN 23       // Your relay pin
   #define BLE_SCAN_TIME 5            // Scan duration
   ```

2. Edit `config/trailer_relay_config.h`:
   ```cpp
   #define LEFT_INDICATOR_PIN 18      // Left turn signal pin
   #define RIGHT_INDICATOR_PIN 19     // Right turn signal pin
   #define BRAKE_LIGHT_PIN 21         // Brake light pin
   ```

### Update MAC Addresses

1. Find ESP32 MAC addresses:
   ```cpp
   // Add this to setup() and upload to each ESP32
   Serial.println(WiFi.macAddress());
   ```

2. Update configuration files with actual MAC addresses:
   ```cpp
   // In trailer_relay_config.h
   #define SNIFFER_MAC_ADDRESS {0x24, 0x6F, 0x28, 0x00, 0x00, 0x01}
   ```

## Step 4: Upload Firmware

### Program the Sniffer ESP32

1. Connect first ESP32 to computer via USB
2. Open `src/bluetooth_sniffer/bluetooth_sniffer.ino`
3. Select correct **Port** in **Tools > Port**
4. Click **Upload** button
5. Monitor serial output at 115200 baud

### Program the Trailer ESP32

1. Connect second ESP32 to computer via USB
2. Open `src/trailer_relay/trailer_relay.ino`
3. Select correct **Port** in **Tools > Port**
4. Click **Upload** button
5. Monitor serial output at 115200 baud

## Step 5: Initial Testing

### Test Communication
1. Power both ESP32 boards
2. Send test commands via Serial Monitor:
   ```
   test            # Run test sequence
   status          # Show system status
   left_on         # Test left indicator
   right_on        # Test right indicator
   all_off         # Turn off all outputs
   ```

### Verify BLE Scanning
1. Monitor sniffer ESP32 serial output
2. Look for BLE device detection messages
3. Test with any BLE device nearby initially

## Step 6: Hardware Integration

### Connect Relay/Driver Circuits
1. Follow wiring diagrams in `docs/wiring/`
2. Use appropriate current ratings for your lights
3. Include fuses for safety
4. Test continuity before applying power

### Install in Vehicle/Trailer
1. Mount ESP32 boards in weatherproof enclosures
2. Connect to vehicle 12V power system
3. Route wiring to trailer lights
4. Secure all connections

## Troubleshooting

### Common Issues

**Upload Fails**
- Check USB cable and drivers
- Hold BOOT button during upload
- Try different USB port
- Reset ESP32 before upload

**BLE Not Working**
- Verify ESP32 board package version
- Check antenna connection
- Try different ESP32 board
- Monitor for error messages

**No Communication Between ESP32s**
- Verify MAC addresses are correct
- Check WiFi channel settings
- Test with serial communication first
- Ensure both devices are powered

**Lights Not Working**
- Check relay/MOSFET connections
- Verify pin assignments
- Test with multimeter
- Check power supply capacity

### Debug Commands

Use these serial commands for debugging:

```
status          # System status
test            # Hardware test
debug_on        # Enable verbose logging
debug_off       # Disable verbose logging
```

## Next Steps

After successful installation:

1. Read [Hardware Setup Guide](hardware_setup.md) for detailed wiring
2. Review [Configuration Guide](configuration.md) for customization
3. Follow [Testing Guide](testing.md) for comprehensive testing
4. Check [Safety Guidelines](safety.md) for important precautions

## Support

For installation issues:
1. Check troubleshooting section above
2. Review Arduino IDE/ESP32 documentation
3. Open GitHub issue with detailed error messages
4. Include serial monitor output and configuration details