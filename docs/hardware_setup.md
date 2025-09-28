# Hardware Setup Guide

This guide covers the physical wiring and hardware connections for the Lumos BBR Bluetooth Sniffer system.

## System Architecture

```
┌─────────────────┐    BLE     ┌──────────────────┐   ESP-NOW   ┌─────────────────┐
│  Lumos Matrix   │──────────► │  Sniffer ESP32   │────────────►│  Trailer ESP32  │
│     Helmet      │            │                  │             │                 │
└─────────────────┘            └──────────────────┘             └─────────────────┘
                                        │                                 │
                                   ┌────▼────┐                      ┌────▼────┐
                                   │ Status  │                      │ Trailer │
                                   │   LED   │                      │ Lights  │
                                   └─────────┘                      └─────────┘
```

## Component Overview

### ESP32 Sniffer Unit
- **Primary Function**: BLE scanning and signal analysis
- **Power**: 5V via USB or 12V-to-5V converter
- **Connections**: Status LED, optional relay output

### ESP32 Trailer Unit  
- **Primary Function**: Light control and safety management
- **Power**: 12V vehicle power with local 5V regulation
- **Connections**: Multiple LED/light outputs, power monitoring

## Wiring Diagrams

### ESP32 Sniffer Connections

```
ESP32 Sniffer Board:
┌─────────────────────────────────┐
│  ESP32-WROOM-32                 │
│                                 │
│  GPIO2  ●────────────────● LED (Built-in)
│  GPIO23 ●────────────┐         │
│  GND    ●────────┐   │         │
│  3V3    ●────┐   │   │         │
└─────────┘    │   │   │         │
               │   │   │         │
            ┌──▼───▼───▼──┐      │
            │   Relay     │      │
            │   Module    │      │
            │  (Optional) │      │
            └─────────────┘      │
                                 │
Power Input:                     │
● 5V USB or 12V-5V converter    │
● GND                           │
```

### ESP32 Trailer Connections

```
ESP32 Trailer Board:
┌─────────────────────────────────┐
│  ESP32-WROOM-32                 │
│                                 │
│  GPIO18 ●─────────● Left Turn LEDs
│  GPIO19 ●─────────● Right Turn LEDs  
│  GPIO21 ●─────────● Brake Lights
│  GPIO22 ●─────────● Hazard Left
│  GPIO23 ●─────────● Hazard Right
│  GPIO2  ●─────────● Status LED
│  GPIO25 ●─────────● Power Relay
│  GND    ●─────────● Common Ground
│  VIN    ●─────────● +12V Input
└─────────────────────────────────┘
```

### LED Driver Circuit (Per Channel)

```
ESP32 GPIO ──────┐
                 │
               ┌─▼─┐
               │2K2│ Resistor
               └─┬─┘
                 │
         Gate  ┌─▼─┐ IRLZ44N MOSFET
      ┌────────┤   │
      │        └─┬─┘
      │          │ Drain
      │          │
    ┌─▼─┐      ┌─▼─┐
    │10K│      │   │ LED Strip
    └─┬─┘      │   │ (+12V)
      │        └─┬─┘
    ──┴──        │ 
     GND       ┌─▼─┐
               │   │ Current
               │   │ Limiting
               └─┬─┘ Resistor
                 │
               ──┴──
                GND
```

## Power System Design

### Power Distribution

```
Vehicle 12V ─────┬─────● Fuse (15A)
                 │
            ┌────▼────┐
            │ Power   │
            │ Switch  │
            │ (opt.)  │
            └────┬────┘
                 │
        ┌────────┼────────┐
        │                 │
   ┌────▼────┐       ┌────▼────┐
   │12V-5V   │       │ Light   │
   │Convert  │       │ Drivers │
   └────┬────┘       └─────────┘
        │
   ┌────▼────┐
   │ ESP32   │
   │ Boards  │
   └─────────┘
```

### Power Requirements

| Component | Voltage | Current | Notes |
|-----------|---------|---------|-------|
| ESP32 Sniffer | 5V | 200mA | USB or converter |
| ESP32 Trailer | 5V | 300mA | With relays active |
| LED Strips (per) | 12V | 1-3A | Depends on length |
| Total System | 12V | 5-8A | With all lights on |

## Detailed Connection Tables

### ESP32 Sniffer Pin Assignments

| GPIO | Function | Connection | Notes |
|------|----------|------------|-------|
| 2 | Status LED | Built-in LED | Activity indicator |
| 23 | Relay Output | Optional relay | For local signaling |
| 0 | Boot/Reset | Button | Programming/reset |
| 21 | SDA | I2C (future) | Expansion |
| 22 | SCL | I2C (future) | Expansion |

### ESP32 Trailer Pin Assignments  

| GPIO | Function | Connection | Max Current |
|------|----------|------------|-------------|
| 18 | Left Turn | MOSFET Gate | 3A @ 12V |
| 19 | Right Turn | MOSFET Gate | 3A @ 12V |
| 21 | Brake Lights | MOSFET Gate | 5A @ 12V |
| 22 | Hazard Left | MOSFET Gate | 3A @ 12V |
| 23 | Hazard Right | MOSFET Gate | 3A @ 12V |
| 2 | Status LED | Built-in LED | System status |
| 25 | Power Relay | Relay coil | Main power control |
| 4 | Emergency Stop | Switch input | Safety shutoff |
| 0 | Manual Override | Switch input | Manual control |

## Component Specifications

### Recommended MOSFETs

**IRLZ44N (Logic Level)**
- VDS: 55V
- ID: 47A
- RDS(on): 17mΩ @ 5V gate
- Package: TO-220

**Alternative: IRLB8721**
- VDS: 30V  
- ID: 62A
- RDS(on): 1.9mΩ @ 4.5V gate
- Package: TO-220

### LED Strip Specifications

**12V LED Strips**
- Voltage: 12V DC
- Current: 1-3A per meter
- Color: White, amber, red as needed
- Protection: IP65 for outdoor use

### Relay Specifications

**5V Coil, DPDT**
- Coil Voltage: 5V DC
- Contact Rating: 10A @ 12V DC
- Mounting: PCB or panel mount

## Assembly Instructions

### Step 1: ESP32 Board Preparation
1. Install headers on ESP32 boards
2. Test basic functionality with simple sketch
3. Verify power consumption with multimeter

### Step 2: Driver Circuit Assembly  
1. Build MOSFET driver circuits on breadboard/PCB
2. Test each channel with LED and resistor
3. Verify gate voltage levels (should be 3.3V)

### Step 3: Power System Assembly
1. Install 12V-5V converter (Buck converter recommended)
2. Add fuses and protection components
3. Test power distribution before connecting ESP32s

### Step 4: Integration Testing
1. Connect one output channel at a time
2. Test with built-in test sequences
3. Verify current draw within specifications

### Step 5: Enclosure and Mounting
1. Use weatherproof enclosures for outdoor installation
2. Provide adequate ventilation for heat dissipation
3. Secure all connections with appropriate connectors

## Safety Considerations

### Electrical Safety
- **Always use appropriate fuses** for each circuit
- **Double-check polarity** before applying power
- **Use proper gauge wire** for current loads
- **Secure all connections** to prevent shorts

### Automotive Integration
- **Follow local regulations** for vehicle modifications
- **Use proper automotive connectors** for permanent installation
- **Consider electromagnetic interference** (EMI) effects
- **Provide emergency override** capabilities

### Heat Management
- **Monitor MOSFET temperatures** under load
- **Use heat sinks** for high-current applications
- **Ensure adequate airflow** in enclosed installations

## Testing Procedures

### Initial Power-Up
1. Check 12V input voltage
2. Verify 5V regulated output
3. Confirm ESP32 boot sequence
4. Test communication between units

### Output Testing
1. Use multimeter to verify output voltages
2. Test each channel individually
3. Check current draw under load
4. Verify proper timing sequences

### System Integration
1. Test with actual LED loads
2. Verify wireless communication
3. Test emergency stop functions
4. Conduct extended operation test

## Troubleshooting Hardware Issues

### No Power
- Check fuses and connections
- Verify power supply output
- Test with multimeter

### Dim or Flickering LEDs
- Check MOSFET gate drive voltage
- Verify current limiting resistors
- Test power supply under load

### Overheating
- Check current draw vs. specifications
- Add heat sinks to MOSFETs
- Improve ventilation

### Communication Issues
- Verify ESP32 power stability
- Check antenna placement
- Test with shorter distances first

## Advanced Features

### Current Monitoring
Add current sensors (ACS712) for overcurrent protection:
```cpp
#define CURRENT_SENSOR_PIN A0
float current = analogRead(CURRENT_SENSOR_PIN) * 0.185; // mV per amp
```

### Temperature Monitoring
Use built-in ESP32 temperature sensor:
```cpp
extern "C" int rom_phy_get_vdd33();
float temperature = (rom_phy_get_vdd33() - 1000) / 10.0;
```

### Voltage Monitoring
Monitor battery voltage:
```cpp
#define VOLTAGE_PIN A1
float voltage = analogRead(VOLTAGE_PIN) * (12.0 / 1024.0);
```

## Next Steps

After hardware setup:
1. Proceed to [Configuration Guide](configuration.md)
2. Follow [Testing Guide](testing.md) for system validation
3. Review [Safety Guidelines](safety.md) for operational safety