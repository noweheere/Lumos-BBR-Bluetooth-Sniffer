# 🛠️ Arduino IDE Setup Guide

Complete setup instructions for developing and uploading the Lumos BBR system.

## 📥 Arduino IDE Installation

### Download and Install Arduino IDE

1. **Download Arduino IDE 2.0+** from [arduino.cc](https://www.arduino.cc/en/software)
2. **Install** following the system-specific instructions
3. **Launch** Arduino IDE and complete initial setup

### Alternative: VS Code with PlatformIO

For advanced users, VS Code with PlatformIO extension provides better development experience:
1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO extension](https://platformio.org/install/ide?install=vscode)
3. Open project folder in VS Code

## 🔧 ESP32 Board Package Setup

### Add ESP32 Board Manager URL

1. Open **Arduino IDE**
2. Go to **File → Preferences**
3. In **Additional Board Manager URLs**, add:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. Click **OK**

### Install ESP32 Board Package

1. Go to **Tools → Board → Boards Manager**
2. Search for **"ESP32"**
3. Install **"ESP32 by Espressif Systems"** (version 2.0.0 or later)
4. Wait for installation to complete

### Select ESP32 Board

1. Go to **Tools → Board → ESP32 Arduino**
2. Select **"ESP32 Dev Module"** or **"ESP32 Wrover Module"**

## 📚 Library Installation

### Required Libraries

Install these libraries through the Library Manager:

#### For BLE Sniffer:
```
ESPAsyncWebServer (by lacamera) - v1.2.3+
AsyncTCP (by dvarrel) - v1.1.1+
ArduinoJson (by Benoit Blanchon) - v6.21.3+
```

#### For LED Controller:
```
FastLED (by Daniel Garcia) - v3.6.0+
ESPAsyncWebServer (by lacamera) - v1.2.3+
AsyncTCP (by dvarrel) - v1.1.1+
ArduinoJson (by Benoit Blanchon) - v6.21.3+
```

### Installation Steps

1. Go to **Tools → Manage Libraries**
2. Search for each library by name
3. Click **Install** for the specified version
4. Wait for installation to complete
5. **Restart Arduino IDE**

### Manual Library Installation (if needed)

If automatic installation fails:

1. Download library ZIP from GitHub
2. Go to **Sketch → Include Library → Add .ZIP Library**
3. Select the downloaded ZIP file
4. Restart Arduino IDE

## ⚙️ Board Configuration

### ESP32 Settings for BLE Sniffer

```
Board: "ESP32 Dev Module"
Upload Speed: "921600"
CPU Frequency: "240MHz (WiFi/BT)"
Flash Frequency: "80MHz"
Flash Mode: "QIO"
Flash Size: "4MB (32Mb)"
Partition Scheme: "Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)"
Core Debug Level: "None"
PSRAM: "Disabled"
Arduino Runs On: "Core 1"
Events Run On: "Core 1"
```

### ESP32 Settings for LED Controller

```
Board: "ESP32 Dev Module"
Upload Speed: "921600"
CPU Frequency: "240MHz (WiFi/BT)"
Flash Frequency: "80MHz"
Flash Mode: "QIO"
Flash Size: "4MB (32Mb)"
Partition Scheme: "Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)"
Core Debug Level: "None"
PSRAM: "Disabled"
Arduino Runs On: "Core 1"
Events Run On: "Core 1"
```

## 🔌 Hardware Connection

### USB Driver Installation

Most ESP32 boards use one of these USB-to-Serial chips:

#### CP2102/CP2104 (most common)
1. Download driver from [Silicon Labs](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
2. Install following system instructions
3. Restart computer if required

#### CH340/CH341
1. Download driver from manufacturer or search "CH340 driver"
2. Install following system instructions
3. May require disabling driver signature enforcement on Windows

#### FTDI (less common)
1. Download from [FTDI website](https://ftdichip.com/drivers/)
2. Install VCP (Virtual COM Port) drivers

### Port Selection

1. Connect ESP32 to computer via USB
2. In Arduino IDE, go to **Tools → Port**
3. Select the correct COM port (Windows) or /dev/ttyUSB* (Linux) or /dev/cu.* (macOS)
4. If no ports appear, check driver installation

## 📁 Project Structure Setup

### Clone or Download Project

```bash
# Using Git
git clone https://github.com/noweheere/Lumos-BBR-Bluetooth-Sniffer.git

# Or download ZIP from GitHub and extract
```

### Opening Projects in Arduino IDE

#### BLE Sniffer:
1. Open **Arduino IDE**
2. Go to **File → Open**
3. Navigate to `Lumos-BBR-Bluetooth-Sniffer/lumos_ble_sniffer/`
4. Select `lumos_ble_sniffer.ino`

#### LED Controller:
1. Open **Arduino IDE** (new window)
2. Go to **File → Open**
3. Navigate to `Lumos-BBR-Bluetooth-Sniffer/trailer_led_controller/`
4. Select `trailer_led_controller.ino`

## 📄 SPIFFS Data Upload

### Install SPIFFS Upload Tool

#### Arduino IDE 1.x:
1. Download **ESP32 Sketch Data Upload** plugin
2. Extract to `[Arduino Sketchbook]/tools/`
3. Restart Arduino IDE
4. Tool appears in **Tools** menu

#### Arduino IDE 2.x:
1. Install **ESP32FS** plugin from extensions
2. Or use command line tool `espfs`

### Upload Web Interface Files

1. Open BLE sniffer project
2. Ensure `data/` folder contains `index.html`
3. Go to **Tools → ESP32 Sketch Data Upload**
4. Wait for upload to complete
5. **Do not** disconnect ESP32 during upload

### Manual SPIFFS Upload (if tool unavailable)

```bash
# Using esptool.py (advanced users)
pip install esptool
cd lumos_ble_sniffer/data
esptool.py --chip esp32 --port COM3 write_flash 0x290000 spiffs.bin
```

## 🚀 Compilation and Upload

### Compile (Verify)

1. Click **Verify** button (✓) or **Ctrl+R**
2. Check for compilation errors in output window
3. Resolve any missing library or syntax errors

### Upload to ESP32

1. Click **Upload** button (→) or **Ctrl+U**
2. Watch for upload progress
3. ESP32 may restart automatically after upload

### Upload Process Troubleshooting

#### "Failed to connect" errors:
1. Hold **BOOT** button on ESP32 during upload
2. Try different USB cable
3. Check COM port selection
4. Reduce upload speed to 115200

#### "Permission denied" on Linux:
```bash
sudo usermod -a -G dialout $USER
# Logout and login again
```

#### macOS permission issues:
```bash
sudo chmod 666 /dev/cu.usbserial-*
```

## 🔍 Serial Monitor Setup

### Configuration

1. Click **Serial Monitor** button or **Ctrl+Shift+M**
2. Set baud rate to **115200**
3. Set line ending to **"Both NL & CR"**

### Expected Output (BLE Sniffer)

```
=== Lumos BBR Bluetooth Sniffer Starting ===
SPIFFS Mount success
BLE initialized
WiFi AP started
IP address: 192.168.4.1
Web server started
Setup complete. Starting BLE scan...
Starting BLE scan...
[12345] Device: iPhone (AA:BB:CC:DD:EE:FF) RSSI: -45 dBm
  Manufacturer Data: 4C 00 01 02 03 04
```

### Expected Output (LED Controller)

```
=== Lumos BBR Trailer LED Controller Starting ===
Running startup LED sequence...
Startup sequence complete
Connecting to WiFi...
WiFi connected!
IP address: 192.168.4.2
Web server started on port 80
Setup complete. Ready for signals.
```

## 🐛 Common Issues and Solutions

### Compilation Errors

#### "BLEDevice.h: No such file or directory"
- **Solution**: Ensure ESP32 board package is installed
- Check board selection (must be ESP32 variant)

#### "AsyncWebServer.h: No such file or directory"
- **Solution**: Install ESPAsyncWebServer library
- Also install AsyncTCP dependency

#### "FastLED.h: No such file or directory"
- **Solution**: Install FastLED library from Library Manager

### Upload Issues

#### "A fatal error occurred: ESP32 chip was not found"
- **Solution**: Check USB connection and drivers
- Try different USB port
- Hold BOOT button during upload

#### "Timed out waiting for packet header"
- **Solution**: Reduce upload speed to 115200
- Check USB cable quality
- Try different computer

### Runtime Issues

#### WiFi AP not appearing
- **Solution**: Check ESP32 power supply
- Ensure WiFi region compatibility
- Try restarting ESP32

#### Web interface not loading
- **Solution**: Verify SPIFFS upload completed
- Check file names match code expectations
- Try clearing browser cache

#### LEDs not working
- **Solution**: Check wiring and power supply
- Verify FastLED configuration matches hardware
- Test with simple LED examples first

## 📊 Performance Optimization

### Memory Usage

Monitor memory usage in Serial Monitor:
```cpp
Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
```

### CPU Usage

For better performance:
- Reduce BLE scan interval if needed
- Optimize LED update frequency
- Use appropriate core assignments

### Power Consumption

To reduce power usage:
- Lower WiFi transmission power
- Reduce LED brightness
- Implement sleep modes when idle

## 🔄 Development Workflow

### Recommended Development Process

1. **Start with examples** - Test basic functionality
2. **Incremental changes** - Add features one at a time
3. **Test frequently** - Upload and test after each change
4. **Use version control** - Git for tracking changes
5. **Document changes** - Comment code modifications

### Code Organization

```
project_folder/
├── main_sketch.ino          # Main Arduino file
├── config.h                 # Configuration constants
├── bluetooth_handler.cpp    # BLE functionality
├── web_server.cpp          # Web interface code
├── led_controller.cpp      # LED control functions
└── data/                   # SPIFFS files
    ├── index.html
    ├── style.css
    └── script.js
```

### Debugging Tips

1. **Use Serial.print()** liberally during development
2. **LED indicators** for visual debugging
3. **Web interface** for real-time monitoring
4. **Logic analyzer** for timing issues
5. **Oscilloscope** for hardware debugging

---

This setup guide ensures a smooth development experience with the Lumos BBR system. Follow each step carefully and refer back to troubleshooting sections as needed.