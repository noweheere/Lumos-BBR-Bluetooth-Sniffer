# Safety Guidelines

⚠️ **CRITICAL SAFETY INFORMATION** ⚠️

This document contains essential safety information for the Lumos BBR Bluetooth Sniffer system. **READ COMPLETELY** before installation, testing, or operation.

## Legal and Regulatory Compliance

### Vehicle Modification Regulations

**Before Installation:**
- **Check local laws** regarding vehicle lighting modifications
- **Verify compatibility** with vehicle inspection requirements  
- **Ensure compliance** with transportation department regulations
- **Consider insurance implications** of vehicle modifications

**Common Requirements:**
- Turn signals must be amber/yellow in most jurisdictions
- Brake lights must be red
- Minimum brightness and visibility standards
- Proper mounting height and positioning
- Manual override capability may be required

### Wireless Communication Compliance

- ESP32 operates in 2.4GHz ISM band (license-free)
- Ensure compliance with local power limits
- Consider interference with other devices
- May require certification for commercial use

## Electrical Safety

### Power System Safety

**⚠️ HIGH CURRENT WARNING:**
- System operates with 12V automotive power (potentially >10A)
- Improper connections can cause fires or damage
- Always use appropriate fuses and circuit protection

**Essential Safety Measures:**
```
1. ALWAYS disconnect battery before wiring
2. Use fuses rated 20% above expected load
3. Verify polarity before connecting power
4. Use automotive-grade wire (minimum 14 AWG for power)
5. Secure all connections with proper terminals
6. Protect wiring from chafing and heat
7. Test with multimeter before applying power
```

### Circuit Protection Requirements

| Circuit | Fuse Rating | Wire Gauge | Notes |
|---------|-------------|------------|-------|
| Main Power | 15A | 12 AWG | Primary protection |
| ESP32 Supply | 5A | 16 AWG | Logic power |
| Each LED Channel | 5A | 16 AWG | Individual protection |
| Control Signals | 1A | 20 AWG | Low current |

### Installation Safety Checklist

**Before Energizing:**
- [ ] All connections secure and insulated
- [ ] No bare wires exposed
- [ ] Fuses installed in all circuits
- [ ] Polarity verified with multimeter
- [ ] No short circuits detected
- [ ] Wiring secured and protected
- [ ] Emergency disconnect accessible

## System Safety Features

### Built-in Safety Mechanisms

**Automatic Safety Systems:**
- **Command Timeout**: Indicators turn off after 3 seconds without signal
- **Watchdog Timer**: System resets if software hangs
- **Emergency Stop**: Hardware button for immediate shutdown
- **Overcurrent Protection**: MOSFETs limit maximum current
- **Voltage Monitoring**: Low voltage detection and shutdown

### Failsafe Operation

**In Case of System Failure:**
```cpp
void emergencyFailsafe() {
    // Turn off all outputs immediately
    digitalWrite(LEFT_INDICATOR_PIN, LOW);
    digitalWrite(RIGHT_INDICATOR_PIN, LOW);
    digitalWrite(BRAKE_LIGHT_PIN, LOW);
    digitalWrite(HAZARD_PIN_1, LOW);
    digitalWrite(HAZARD_PIN_2, LOW);
    
    // Cut main power relay
    digitalWrite(POWER_RELAY_PIN, LOW);
    
    // Signal emergency state
    while(true) {
        digitalWrite(STATUS_LED_PIN, HIGH);
        delay(100);
        digitalWrite(STATUS_LED_PIN, LOW);
        delay(100);
    }
}
```

### Manual Override System

**Implementation:**
- Physical switch to bypass electronic control
- Direct connection to vehicle's turn signal system
- Independent power feed for emergency operation
- Clear indication when override is active

```
Manual Override Circuit:
Vehicle Turn Signal ──┬── Normal ESP32 Control
                      │
                    [Override]
                    [Switch  ]
                      │
                      └── Direct to Trailer Lights
```

## Operational Safety

### Pre-Operation Checks

**Every Use:**
- [ ] System status LED indicates normal operation
- [ ] All connections secure
- [ ] Emergency stop button functional
- [ ] Manual override switch operational
- [ ] Adequate power supply voltage (>11.5V)
- [ ] No error messages on serial output

### Operating Limits

**Environmental Limits:**
- Operating Temperature: -20°C to +60°C
- Storage Temperature: -30°C to +70°C  
- Humidity: 10% to 90% non-condensing
- Vibration: Standard automotive levels
- Water Resistance: IP65 recommended for enclosures

**Electrical Limits:**
- Input Voltage: 10V to 16V DC (12V nominal)
- Maximum Current per Channel: 5A
- Total System Current: 15A maximum
- Continuous Operation: 8 hours maximum without cooling break

### Usage Warnings

**⚠️ DO NOT:**
- Exceed maximum current ratings
- Operate without proper fuses
- Use in explosive atmospheres
- Modify safety-critical code without testing
- Override safety timeouts
- Use damaged components
- Install while vehicle is running

**⚠️ ALWAYS:**
- Test system before each use
- Monitor for overheating
- Keep manual override accessible
- Maintain clear line of sight to trailer
- Follow traffic laws and signals
- Report malfunctions immediately

## Maintenance Safety

### Regular Inspections

**Weekly (Heavy Use) / Monthly (Light Use):**
- Visual inspection of all connections
- Check for wire chafing or damage
- Verify fuse integrity
- Test emergency stop function
- Clean connectors and terminals
- Check enclosure seals

**Annually:**
- Complete system electrical test
- Replace fuses (preventive)
- Update firmware if available
- Professional inspection recommended
- Documentation review and update

### Safe Maintenance Procedures

**Before Maintenance:**
1. Turn off vehicle and remove key
2. Disconnect negative battery terminal
3. Wait 5 minutes for capacitor discharge
4. Use lockout/tagout procedures if multiple people involved
5. Test circuits are de-energized with multimeter

**During Maintenance:**
- Use proper PPE (safety glasses, insulated tools)
- One person working on electrical at a time
- Keep fire extinguisher nearby
- Work in well-ventilated area
- Document all changes made

## Emergency Procedures

### System Malfunction

**Immediate Actions:**
1. **STOP VEHICLE** safely when possible
2. **ACTIVATE** manual override switch
3. **TURN OFF** system power switch
4. **USE HAND SIGNALS** until repairs completed
5. **CONTACT** qualified technician

### Fire or Smoke

**Emergency Response:**
1. **STOP** vehicle immediately
2. **DISCONNECT** battery if safe to do so
3. **USE** appropriate fire extinguisher (Class C electrical)
4. **EVACUATE** area until safe
5. **CONTACT** emergency services if necessary

### Electrical Shock

**First Aid:**
1. **DO NOT** touch victim if still connected to power
2. **TURN OFF** power at source
3. **CALL** emergency services immediately
4. **PROVIDE** first aid if trained
5. **DOCUMENT** incident for investigation

## Installation Safety

### Professional Installation Recommended

**Consider Professional Installation If:**
- Not experienced with automotive electrical systems
- Local regulations require certified installation
- Insurance requirements mandate professional work
- Complex integration with existing vehicle systems
- Commercial or fleet applications

### DIY Installation Requirements

**Minimum Qualifications:**
- Basic electrical knowledge and skills
- Experience with automotive systems
- Proper tools and test equipment
- Understanding of local regulations
- Ability to read wiring diagrams

**Required Tools:**
- Digital multimeter
- Crimping tools for automotive connectors
- Heat gun for heat shrink tubing
- Wire strippers and cutters
- Soldering iron (if needed)
- Basic hand tools

### Installation Environment

**Safe Installation Conditions:**
- Well-lit work area
- Stable vehicle support (jack stands, not just jack)
- Fire extinguisher accessible
- First aid kit available
- Another person nearby (buddy system)
- Proper ventilation if soldering

## Testing Safety

### Safe Testing Procedures

**Bench Testing:**
- Use current-limited power supply
- Start with reduced voltage (6V)
- Test one channel at a time
- Monitor temperature of components
- Have emergency shutdown ready

**Vehicle Testing:**
- Test in safe, controlled environment
- Have manual override ready
- Start with engine off
- Monitor all outputs with meter
- Test emergency stop functions

### Test Equipment Safety

**Required Safety Equipment:**
- Safety glasses
- Insulated test probes
- Current-limited power supplies
- Fused test leads
- Ground fault protection

## Data and Privacy

### Information Security

**Data Collection:**
- System logs may contain location information
- BLE scanning detects nearby devices
- Consider privacy implications
- Secure data transmission channels

**Recommendations:**
- Regular firmware updates
- Strong passwords for web interfaces
- Secure wireless channels
- Data encryption for sensitive applications

## Training and Competency

### User Training Requirements

**Basic Users:**
- System operation procedures
- Emergency shutdown procedures
- Basic troubleshooting
- Safety precautions
- Legal requirements

**Maintenance Personnel:**
- Advanced troubleshooting
- Safe maintenance procedures
- Component replacement
- System testing
- Documentation updates

### Certification

**Professional Certification Recommended For:**
- Commercial installations
- Fleet applications
- Safety-critical applications
- Insurance compliance
- Regulatory compliance

## Incident Reporting

### When to Report

**Report These Incidents:**
- Any safety-related malfunction
- Electrical fires or smoke
- Component failures
- Injury or near-miss events
- Regulatory compliance issues

### Reporting Information

**Include in Reports:**
- Date, time, and location
- System configuration and version
- Environmental conditions
- Sequence of events
- Actions taken
- Injuries or damage
- Corrective actions needed

### Contact Information

**Emergency:**
- Local emergency services: [Local Number]
- Poison control: [Local Number]
- Fire department: [Local Number]

**Technical Support:**
- Project GitHub Issues
- Manufacturer support (if applicable)
- Local automotive electrical specialist

## Disclaimer and Liability

### Important Legal Notice

**⚠️ USER RESPONSIBILITY:**
This system is provided for educational and research purposes. Users assume all responsibility for:
- Compliance with local laws and regulations
- Safe installation and operation
- Proper maintenance and inspection
- Consequences of system failure or misuse

**No Warranty:**
This system is provided "as-is" without warranty of any kind. The developers disclaim all liability for damages, injuries, or legal consequences resulting from use of this system.

**Professional Review Recommended:**
Have system reviewed by qualified automotive electrical professional before installation, especially for safety-critical applications.

---

## Safety Checklist Summary

**Before Each Use:**
- [ ] System status normal
- [ ] Emergency stop functional
- [ ] Manual override available
- [ ] All connections secure
- [ ] No error indicators
- [ ] Adequate power supply

**Installation Safety:**
- [ ] Professional review completed
- [ ] Local regulations checked
- [ ] Proper fuses installed
- [ ] Emergency procedures known
- [ ] Documentation complete
- [ ] Training completed

**Maintenance Safety:**
- [ ] Regular inspections scheduled
- [ ] Maintenance procedures documented
- [ ] Emergency contacts available
- [ ] Spare parts inventory
- [ ] Update procedures established

**Remember: Safety is everyone's responsibility. When in doubt, consult a professional.**