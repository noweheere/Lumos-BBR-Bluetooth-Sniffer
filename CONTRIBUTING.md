# 🤝 Contributing to Lumos BBR Bluetooth Sniffer

Thank you for your interest in contributing to the Lumos BBR project! This document provides guidelines for contributing to the project.

## 🎯 How to Contribute

### Areas Where We Need Help

1. **BLE Protocol Reverse Engineering**
   - Discovering actual Lumos helmet BLE patterns
   - Testing with different Lumos helmet models
   - Documenting signal patterns and timing

2. **Hardware Design**
   - PCB layouts and schematics
   - 3D printable enclosures
   - Improved antenna designs
   - Power optimization circuits

3. **Software Development**
   - Enhanced pattern recognition algorithms
   - Better web interface design
   - Mobile app development
   - Performance optimizations

4. **Documentation**
   - User guides and tutorials
   - Video demonstrations
   - Translation to other languages
   - Hardware assembly guides

5. **Testing and Validation**
   - Testing with different ESP32 boards
   - Environmental testing (weather, temperature)
   - Range and performance testing
   - Battery life optimization

## 🛠️ Development Setup

### Prerequisites

1. **Hardware**:
   - ESP32 development board(s)
   - LED strips (WS2812B recommended)
   - Basic electronics tools and components

2. **Software**:
   - Arduino IDE 2.0+ with ESP32 support
   - Git for version control
   - Text editor or IDE of choice

### Getting Started

1. **Fork the Repository**
   ```bash
   # Click "Fork" on GitHub, then clone your fork
   git clone https://github.com/YOUR_USERNAME/Lumos-BBR-Bluetooth-Sniffer.git
   cd Lumos-BBR-Bluetooth-Sniffer
   ```

2. **Set Up Development Environment**
   - Follow the [Arduino IDE Setup Guide](arduino_ide_setup.md)
   - Install required libraries
   - Test compilation of both sketches

3. **Create a Branch**
   ```bash
   git checkout -b feature/your-feature-name
   # or
   git checkout -b bugfix/issue-description
   ```

## 📝 Contribution Guidelines

### Code Style

#### Arduino/C++ Code
- Use consistent indentation (2 spaces)
- Follow Arduino naming conventions
- Comment complex algorithms
- Use meaningful variable names

```cpp
// Good
const int LED_PIN = 2;
unsigned long lastUpdateTime = 0;

void updateLEDPattern() {
  // Update LED animation based on current pattern
  if (millis() - lastUpdateTime > UPDATE_INTERVAL) {
    // Implementation here
    lastUpdateTime = millis();
  }
}

// Avoid
int p = 2;
unsigned long t = 0;
```

#### Documentation
- Use clear, concise language
- Include code examples where appropriate
- Keep README files up to date
- Document hardware connections and requirements

### Commit Messages

Use clear, descriptive commit messages:

```bash
# Good
git commit -m "Add support for SK6812 LED strips"
git commit -m "Fix WiFi reconnection issue in LED controller"
git commit -m "Update wiring diagram for rev 2 hardware"

# Avoid
git commit -m "fix bug"
git commit -m "update"
```

### Pull Request Process

1. **Ensure your code compiles** without warnings
2. **Test thoroughly** with actual hardware if possible
3. **Update documentation** if you've changed functionality
4. **Write clear PR description** explaining:
   - What problem does this solve?
   - How was it tested?
   - Any breaking changes?

#### PR Template
```markdown
## Description
Brief description of the changes.

## Type of Change
- [ ] Bug fix (non-breaking change which fixes an issue)
- [ ] New feature (non-breaking change which adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to not work as expected)
- [ ] Documentation update

## Testing
- [ ] Compiled successfully
- [ ] Tested on hardware
- [ ] Tested with actual Lumos helmet (if applicable)

## Hardware Tested
- ESP32 model: 
- LED strip type:
- Other components:

## Checklist
- [ ] My code follows the project's style guidelines
- [ ] I have performed a self-review of my own code
- [ ] I have commented my code, particularly in hard-to-understand areas
- [ ] I have made corresponding changes to the documentation
- [ ] My changes generate no new warnings
```

## 🔬 Reverse Engineering Guidelines

### Ethical Considerations

- **Educational Purpose**: This project is for educational and interoperability purposes
- **No Harm**: Don't attempt to hack or damage Lumos devices
- **Respect IP**: Don't copy copyrighted code or violate patents
- **Safety First**: Ensure modifications don't compromise bicycle safety

### BLE Analysis Process

1. **Passive Monitoring Only**
   - Only capture broadcast advertisements
   - Don't attempt to connect to or control Lumos devices
   - Document observed patterns objectively

2. **Data Collection**
   ```cpp
   // Document findings in this format
   struct LumosPattern {
     String trigger;           // "left_turn_button_press"
     String manufacturerData;  // "4C 00 01 02 03"
     int rssi;                // Signal strength
     String notes;            // Additional observations
   };
   ```

3. **Pattern Documentation**
   - Record multiple samples of each pattern
   - Note timing and duration
   - Test reproducibility
   - Document confidence level

### Sharing Discoveries

When you discover new patterns:

1. **Document thoroughly** in the [reverse engineering guide](docs/reverse_engineering_guide.md)
2. **Provide test data** with multiple samples
3. **Include confidence assessment** (High/Medium/Low)
4. **Share implementation code** for pattern detection

## 🧪 Testing Guidelines

### Hardware Testing

Before submitting hardware-related changes:

1. **Component Testing**
   - Verify all connections are correct
   - Test with recommended components
   - Document any component substitutions

2. **System Testing**  
   - Test complete system integration
   - Verify power consumption is reasonable
   - Test in realistic environmental conditions

3. **Safety Testing**
   - Ensure no fire/shock hazards
   - Test thermal characteristics
   - Verify proper fusing and protection

### Software Testing

1. **Compilation Testing**
   ```bash
   # Test both sketches compile without errors
   arduino-cli compile --fqbn esp32:esp32:esp32 lumos_ble_sniffer/
   arduino-cli compile --fqbn esp32:esp32:esp32 trailer_led_controller/
   ```

2. **Functional Testing**
   - Test all advertised features
   - Verify web interface functionality
   - Test error handling and recovery

3. **Performance Testing**
   - Monitor memory usage over time
   - Test BLE scanning performance
   - Verify WiFi stability

## 🐛 Bug Reports

### Before Reporting

1. **Search existing issues** to avoid duplicates
2. **Test with latest code** from main branch
3. **Try with minimal configuration** to isolate the issue

### Bug Report Template

```markdown
**Describe the bug**
A clear and concise description of what the bug is.

**To Reproduce**
Steps to reproduce the behavior:
1. Go to '...'
2. Click on '....'
3. Scroll down to '....'
4. See error

**Expected behavior**
A clear and concise description of what you expected to happen.

**Hardware:**
- ESP32 model: [e.g. ESP32-WROOM-32]
- LED strips: [e.g. WS2812B, 60 LEDs/m]
- Power supply: [e.g. 12V 5A]

**Software:**
- Arduino IDE version: [e.g. 2.0.3]
- ESP32 board package version: [e.g. 2.0.6]
- Library versions: [e.g. FastLED 3.6.0]

**Additional context**
Add any other context about the problem here, including:
- Serial monitor output
- Photos of hardware setup
- Scope traces if available
```

## 🎖️ Recognition

Contributors will be recognized in:
- Project README
- Code comments for significant contributions
- Release notes
- Hall of Fame for major contributions

### Types of Recognition

- **Code Contributors**: Direct code contributions
- **Hardware Designers**: PCB layouts, enclosures, schematics
- **Reverse Engineers**: BLE pattern discoveries
- **Testers**: Thorough testing and validation
- **Documenters**: Guides, tutorials, translations

## 📞 Communication

### Getting Help

- **GitHub Discussions**: For general questions and design discussion
- **GitHub Issues**: For specific bugs or feature requests
- **Code Comments**: For implementation questions

### Project Roadmap

Check the [GitHub Projects](https://github.com/noweheere/Lumos-BBR-Bluetooth-Sniffer/projects) tab for:
- Planned features
- Current priorities
- Help wanted items

## 📄 License

By contributing to this project, you agree that your contributions will be licensed under the MIT License that covers the project. You also confirm that you have the right to contribute the code/documentation/designs you're submitting.

---

Thank you for contributing to the Lumos BBR project! Every contribution, no matter how small, helps make bicycle safety technology more accessible to everyone.