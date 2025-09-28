# Lumos BBR Bluetooth Sniffer

ESP32 Bluetooth sniffer for Lumos Matrix helmet communication analysis and trailer indicator relay system.

This project provides a complete solution for intercepting Bluetooth Low Energy (BLE) communications from Lumos Matrix smart bicycle helmets and relaying turn signal, brake light, and hazard commands to trailer indicator systems.

## Features

- **Real-time BLE Scanning**: Continuously monitors for Lumos Matrix helmet signals
- **Signal Pattern Analysis**: Decodes turn signals, brake lights, and hazard patterns
- **Dual ESP32 System**: Separate sniffer and trailer relay controllers
- **Multiple Communication Methods**: ESP-NOW, WiFi, and Serial communication options
- **Comprehensive Safety Features**: Timeout protection, emergency stop, failsafe modes  
- **Web Configuration Interface**: Easy setup and monitoring via web browser
- **Extensive Logging**: JSON-formatted logs for debugging and analysis
- **Hardware Flexibility**: Configurable pin assignments and timing parameters

## System Overview

```
[Lumos Matrix Helmet] → BLE → [ESP32 Sniffer] → ESP-NOW/WiFi → [ESP32 Trailer Relay] → [Trailer Lights]
```

### Components

1. **Bluetooth Sniffer ESP32**: Captures and analyzes Lumos helmet BLE advertisements
2. **Trailer Relay ESP32**: Controls trailer indicator lights based on received commands
3. **Trailer Light Hardware**: LED strips, indicators, brake lights, and hazard lights

## Quick Start

### Hardware Requirements

- 2x ESP32 development boards (ESP32-WROOM-32 recommended)
- LED strips or indicator lights for trailer
- Relay modules or MOSFET drivers for high-current loads
- 12V power supply for trailer system
- Breadboard/PCB for connections

### Software Setup

1. **Install Arduino IDE** with ESP32 board support
2. **Install Required Libraries** (see `src/*/libraries.txt`)
3. **Upload Firmware**:
   - Upload `src/bluetooth_sniffer/bluetooth_sniffer.ino` to sniffer ESP32
   - Upload `src/trailer_relay/trailer_relay.ino` to trailer ESP32
4. **Configure Settings** in `config/*.h` files

### Basic Configuration

1. Update MAC addresses in configuration files
2. Set appropriate pin assignments for your hardware
3. Adjust timing and safety parameters as needed
4. Test system with built-in test sequences

## Documentation

- [📖 **Installation Guide**](docs/installation.md) - Detailed setup instructions
- [🔧 **Hardware Setup**](docs/hardware_setup.md) - Wiring diagrams and connections
- [⚙️ **Configuration**](docs/configuration.md) - Software configuration options
- [🧪 **Testing Guide**](docs/testing.md) - Testing procedures and troubleshooting
- [📡 **API Reference**](docs/api_reference.md) - Command protocols and data formats
- [🔒 **Safety Guidelines**](docs/safety.md) - Important safety considerations

## Project Structure

```
├── src/
│   ├── bluetooth_sniffer/     # Main BLE sniffer ESP32 code
│   └── trailer_relay/         # Trailer relay controller ESP32 code
├── config/                    # Configuration headers
├── docs/                      # Documentation
│   ├── images/               # Wiring diagrams and photos
│   └── wiring/               # Circuit diagrams
├── examples/                  # Example configurations
└── tests/                    # Test procedures and scripts
```

## Contributing

Contributions are welcome! Please read our contributing guidelines and submit pull requests for any improvements.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Disclaimer

This project is for educational and research purposes. Users are responsible for compliance with local regulations regarding vehicle modifications and wireless communications.

## Support

For questions, issues, or feature requests, please open an issue on GitHub.
