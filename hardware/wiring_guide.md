# 🔌 Hardware Wiring Guide

Complete wiring instructions for the Lumos BBR Bluetooth Sniffer system.

## 📋 Components List

### BLE Sniffer Unit
| Component | Quantity | Specifications | Notes |
|-----------|----------|----------------|-------|
| ESP32 DevKit V1 | 1 | 38-pin development board | Primary controller |
| Status LED | 1 | 5mm LED, any color | Optional (built-in LED available) |
| Resistor | 1 | 220Ω, 1/4W | For external status LED |
| USB Cable | 1 | Micro-USB or USB-C | Programming and power |
| Enclosure | 1 | IP65 rated, min 100x60x30mm | Weather protection |

### Trailer LED Controller Unit
| Component | Quantity | Specifications | Notes |
|-----------|----------|----------------|-------|
| ESP32 DevKit V1 | 1 | 38-pin development board | LED controller |
| WS2812B LED Strip | 3 | Individually addressable | Left: 20, Right: 20, Brake: 30 LEDs |
| Push Buttons | 3 | 12mm tactile switches | Manual override controls |
| Pull-up Resistors | 3 | 10kΩ, 1/4W | Button debouncing |
| Power Capacitor | 1 | 1000µF, 16V electrolytic | LED strip power filtering |
| Voltage Regulator | 1 | 5V, 3A step-down | Buck converter module |
| Battery Monitor | 1 | Voltage divider circuit | Battery level sensing |
| 12V Battery | 1 | 5Ah lithium or SLA | Main power source |
| Enclosure | 1 | IP67 rated, min 150x100x50mm | Trailer mounting box |
| Connectors | Various | IP65 rated | Cable connections |

## 🔧 BLE Sniffer Unit Wiring

### Basic Setup (Minimal)
```
ESP32 DevKit V1 Pinout:
                    ┌─────────────────┐
                    │      ESP32      │
                    │    DevKit V1    │
                    │                 │
         3V3 ●──────┤ 3V3         VIN ├──────● VIN (5V)
         GND ●──────┤ GND        GPIO23├──────● 
         GPIO15 ●───┤ GPIO15    GPIO22 ├──────● 
         GPIO2 ●────┤ GPIO2     GPIO1  ├──────● TX
         GPIO0 ●────┤ GPIO0     GPIO3  ├──────● RX
         GPIO4 ●────┤ GPIO4     GPIO21 ├──────● 
         GPIO16 ●───┤ GPIO16    GPIO19 ├──────● 
         GPIO17 ●───┤ GPIO17    GPIO18 ├──────● 
         GPIO5 ●────┤ GPIO5         5V ├──────● 5V
         GPIO18 ●───┤ GPIO18   GPIO13 ├──────● 
         GPIO19 ●───┤ GPIO19   GPIO12 ├──────● 
         GPIO21 ●───┤ GPIO21   GPIO14 ├──────● 
         GPIO3 ●────┤ RX       GPIO27 ├──────● 
         GPIO1 ●────┤ TX       GPIO26 ├──────● 
         GPIO22 ●───┤ GPIO22   GPIO25 ├──────● 
         GPIO23 ●───┤ GPIO23   GPIO33 ├──────● 
                    │                 │
                    └─────────────────┘

Connections:
- GPIO2: Built-in LED (no external connection needed)
- USB: Programming and power
```

### Advanced Setup (with External Components)
```
External Status LED:
ESP32 GPIO2 ──── [220Ω] ──── LED(+) ──── LED(-) ──── GND

Battery Monitoring (optional):
12V Battery(+) ──── [10kΩ] ──── ESP32 A0 ──── [10kΩ] ──── GND
                           │
                      (Voltage divider gives 0-3.3V for 0-6.6V input)

External WiFi Antenna (optional):
- Use u.FL to SMA adapter if better range needed
```

## 🚦 LED Controller Unit Wiring

### Power Distribution
```
12V Battery System:
                    ┌─────────────────┐
       12V Battery  │                 │
           │        │   Buck Converter │    5V, 3A Output
           ├────────┤ IN+         OUT+ ├─────────┬──── ESP32 VIN
           │        │                 │         │
           ├────────┤ IN-         OUT- ├─────────┼──── Common GND
           │        │                 │         │
    [Fuse 5A]       └─────────────────┘         │
           │                                    │
         GND ──────────────────────────────────┘

Capacitor Filtering:
5V ────── [1000µF Cap+] ────── LED Strip Power
GND ───── [1000µF Cap-] ────── Common GND
```

### ESP32 to LED Strip Connections
```
LED Strip Wiring:
                    ┌─────────────────┐
                    │      ESP32      │
                    │                 │
Left Turn Strip:    │                 │
5V ●────────────────┤ VIN             │
Data ●──────────────┤ GPIO18          │
GND ●───────────────┤ GND             │
                    │                 │
Right Turn Strip:   │                 │
5V ●────────────────┤ VIN             │
Data ●──────────────┤ GPIO19          │
GND ●───────────────┤ GND             │
                    │                 │
Brake Strip:        │                 │
5V ●────────────────┤ VIN             │
Data ●──────────────┤ GPIO21          │
GND ●───────────────┤ GND             │
                    └─────────────────┘

WS2812B LED Strip Pinout (typical):
┌─────┐
│ ●   │ = 5V (Red wire)
│ ●   │ = Data In (Green/White wire) 
│ ●   │ = GND (Black wire)
└─────┘
```

### Button Wiring
```
Button Connections (with internal pull-ups):
                    ┌─────────────────┐
                    │      ESP32      │
Left Button:        │                 │
                    │                 │ GPIO12 ●─────[Button]─── GND
                    │                 │
Right Button:       │                 │ GPIO13 ●─────[Button]─── GND
                    │                 │
Hazard Button:      │                 │ GPIO14 ●─────[Button]─── GND
                    │                 │
                    └─────────────────┘

Button Schematic (per button):
ESP32 GPIO ────[10kΩ]──── 3.3V (internal pull-up enabled in code)
    │
    └──── [Button] ──── GND

When button pressed: GPIO reads LOW (0V)
When button released: GPIO reads HIGH (3.3V)
```

### Battery Monitoring Circuit
```
Battery Voltage Monitoring:
12V Battery(+) ──── [47kΩ] ──── ESP32 A0 ──── [10kΩ] ──── GND
                           │
                    (Voltage divider: 12V → 2.1V max)

Calculation:
- 12V battery → 2.1V at A0 (safe for ESP32's 3.3V max)
- 10V battery → 1.75V at A0 (low battery warning)
- A0 reading: 0-4095 (12-bit ADC)
- Voltage = A0_reading * (3.3/4095) * (57/10)
```

## 📐 PCB Layout Recommendations

### BLE Sniffer PCB
```
Simple single-layer PCB layout:
┌─────────────────────────────────┐
│  [USB Connector]                │
│                                 │
│         [ESP32 Module]          │
│                                 │
│  [Status LED]  [Resistor]       │
│                                 │
│  [Power Indicator LED]          │
│                                 │
│  [Mounting Holes]               │
└─────────────────────────────────┘

Size: 60mm x 40mm
```

### LED Controller PCB
```
Two-layer PCB layout:
┌─────────────────────────────────────────┐
│ [Power In] [Fuse] [Buck Converter]      │
│                                         │
│            [ESP32 Module]               │
│                                         │
│ [LED Out 1] [LED Out 2] [LED Out 3]     │
│                                         │
│ [Button 1] [Button 2] [Button 3]        │
│                                         │
│ [Status LEDs] [Test Points]             │
│                                         │
│ [Mounting Holes]                        │
└─────────────────────────────────────────┘

Size: 100mm x 70mm
```

## 🔋 Power Calculations

### BLE Sniffer Power Requirements
```
Component Power Analysis:
- ESP32 (WiFi + BLE active): 200mA @ 3.3V = 0.66W
- Status LED: 20mA @ 3.3V = 0.066W
- Total: 220mA @ 5V = 1.1W

Battery Life Calculation:
- 5000mAh USB power bank @ 5V
- Runtime: 5000mAh / 220mA = 22.7 hours
- Practical runtime: ~18 hours (efficiency losses)
```

### LED Controller Power Requirements
```
Component Power Analysis:
- ESP32: 200mA @ 5V = 1W
- LED strips (all on, full brightness):
  * Left: 20 LEDs × 60mA = 1.2A
  * Right: 20 LEDs × 60mA = 1.2A  
  * Brake: 30 LEDs × 60mA = 1.8A
  * Total LEDs: 4.2A @ 5V = 21W
- Total system: 4.4A @ 5V = 22W

Battery Life Calculation (typical use):
- 5Ah 12V battery = 60Wh capacity
- Average power (30% LED duty cycle): ~8W
- Runtime: 60Wh / 8W = 7.5 hours
- Conservative estimate: 6 hours
```

## 🛡️ Protection Circuits

### Overcurrent Protection
```
Fuse Selection:
- Main 12V input: 5A slow-blow fuse
- LED strip feeds: Individual 2A fast-blow fuses
- ESP32 feed: 1A fast-blow fuse

Fuse Locations:
12V Battery ──[5A Fuse]── Buck Converter ──[1A Fuse]── ESP32
             │
             └──[2A Fuse]── LED Strip Power Bus
```

### ESD Protection
```
ESD Protection for Exposed Connectors:
Data Lines ──[TVS Diode]── GND
Power Lines ──[TVS Diode]── GND

TVS Diode Selection:
- Data lines: 5.6V, 600W TVS
- Power lines: 15V, 1500W TVS
```

### Reverse Polarity Protection
```
Reverse Polarity Protection:
Battery(+) ──[Schottky Diode]── System(+)
Battery(-) ─────────────────── System(-)

Diode Selection:
- Schottky diode: 40V, 5A, low Vf
- Alternative: P-channel MOSFET for lower losses
```

## 🌡️ Thermal Management

### Heat Dissipation
```
Component Thermal Analysis:
- Buck converter: 2-3W dissipation
- ESP32: 0.5W typical
- LED strips: Distributed heat

Cooling Requirements:
- Buck converter: Small heatsink recommended
- ESP32: Natural convection sufficient
- LED strips: Aluminum backing for heat spreading
```

### Temperature Monitoring
```
Optional Temperature Sensing:
DS18B20 Temperature Sensor:
3.3V ──[4.7kΩ]── Data ── ESP32 GPIO
GND ──────────── GND
VCC ──────────── 3.3V

Code addition for monitoring:
```cpp
#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMP_SENSOR_PIN 25
OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature sensors(&oneWire);

void checkTemperature() {
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);
    
    if (temp > 60.0) {
        // Reduce LED brightness for thermal protection
        FastLED.setBrightness(LED_BRIGHTNESS / 2);
        Serial.println("Thermal protection activated");
    }
}
```
```

## 🔧 Assembly Instructions

### Step 1: PCB Preparation
1. Inspect PCB for defects
2. Clean with isopropyl alcohol
3. Apply flux to solder pads

### Step 2: Component Mounting Order
1. **Resistors and small components**
2. **Integrated circuits and modules**
3. **Connectors and mechanical components**
4. **Wire harnesses and cables**

### Step 3: Testing Procedure
1. **Visual inspection** - check for solder bridges
2. **Continuity testing** - verify connections
3. **Power-on test** - check voltages
4. **Functional test** - run test firmware

### Step 4: Enclosure Assembly
1. **Install PCB** in enclosure with spacers
2. **Route cables** through appropriate openings
3. **Install connectors** in panel cutouts
4. **Apply sealant** to cable entries
5. **Final assembly** and testing

## 🧰 Tools Required

### Basic Tools
- Soldering iron (temperature controlled)
- Solder (60/40 or lead-free)
- Flux
- Desoldering braid
- Wire strippers
- Crimping tools
- Heat shrink tubing
- Multimeter

### Advanced Tools
- Hot air rework station
- Oscilloscope (for debugging)
- Logic analyzer
- Power supply (variable)
- Function generator (for testing)

## 🔍 Troubleshooting

### Common Assembly Issues

**LED strips not working:**
- Check power supply voltage (should be 4.8-5.2V)
- Verify data line connections
- Test with simple FastLED example
- Check for reversed polarity

**ESP32 not programming:**
- Verify USB connection
- Check ESP32 power (3.3V on VCC pin)
- Try holding BOOT button during programming
- Check for solder bridges on programming pins

**WiFi connection problems:**
- Check antenna connection
- Verify ESP32 power supply stability
- Test in different WiFi environment
- Check for interference from LED strips

**Button inputs not working:**
- Verify pull-up resistor values
- Check button contact integrity
- Test with multimeter
- Verify GPIO pin assignments in code

## 📋 Quality Control Checklist

### Pre-Assembly Inspection
- [ ] All components received and correct values
- [ ] PCBs inspected for defects
- [ ] Tools and supplies ready

### Assembly Verification
- [ ] All components properly oriented
- [ ] Solder joints inspected and reworked if needed
- [ ] No cold solder joints or bridges
- [ ] Flux cleaned from PCB

### Electrical Testing
- [ ] Power supply voltages correct
- [ ] Current consumption within limits
- [ ] All GPIO pins functional
- [ ] Communication interfaces working

### Final System Test
- [ ] Complete firmware upload successful
- [ ] All features tested and functional
- [ ] Performance meets specifications
- [ ] Documentation complete

---

This wiring guide provides complete instructions for building a reliable Lumos BBR system. Follow safety procedures and double-check all connections before applying power.