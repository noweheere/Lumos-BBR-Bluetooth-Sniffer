# 🛒 Parts List and Bill of Materials

Complete parts list for building the Lumos BBR Bluetooth Sniffer system.

## 💰 Cost Summary

| System Component | Estimated Cost | Notes |
|------------------|----------------|-------|
| BLE Sniffer Unit | $25-35 | Basic version |
| LED Controller Unit | $85-120 | With 70 LEDs total |
| Enclosures & Mounting | $30-50 | Weather-resistant |
| **Total System Cost** | **$140-205** | DIY build |

*Costs are estimates in USD as of 2024. Prices may vary by supplier and location.*

## 🔧 BLE Sniffer Unit (Main Scanner)

### Core Components
| Part | Specification | Quantity | Unit Cost | Supplier | Part Number |
|------|---------------|----------|-----------|----------|-------------|
| ESP32 DevKit V1 | 38-pin, WiFi+BLE | 1 | $8-12 | Multiple | ESP-WROOM-32 |
| Micro-USB Cable | Data + Power | 1 | $3-5 | Multiple | Various |
| Status LED | 5mm, Red/Green | 1 | $0.10 | Digikey | LTL-4233 |
| Resistor | 220Ω, 1/4W | 1 | $0.05 | Digikey | CF14JT220R |

### Optional Components
| Part | Specification | Quantity | Unit Cost | Purpose |
|------|---------------|----------|-----------|---------|
| External Antenna | 2.4GHz, u.FL | 1 | $5-10 | Better range |
| u.FL to SMA Adapter | Right angle | 1 | $2-3 | Antenna connection |
| Voltage Regulator | 3.3V LDO | 1 | $1-2 | Battery operation |
| Battery Connector | JST-PH 2-pin | 1 | $0.50 | Battery power |

### Enclosure & Mounting
| Part | Specification | Quantity | Unit Cost | Supplier |
|------|---------------|----------|-----------|----------|
| Enclosure | IP65, 100x60x30mm | 1 | $8-15 | Hammond/Bud |
| Cable Gland | PG7, IP68 | 2 | $1-2 each | Multiple |
| Mounting Screws | M3x10mm SS | 4 | $0.10 each | McMaster |
| PCB Spacers | M3x10mm | 4 | $0.25 each | McMaster |

## 🚦 LED Controller Unit (Trailer Lights)

### Core Components
| Part | Specification | Quantity | Unit Cost | Supplier | Part Number |
|------|---------------|----------|-----------|----------|-------------|
| ESP32 DevKit V1 | 38-pin, WiFi+BLE | 1 | $8-12 | Multiple | ESP-WROOM-32 |
| WS2812B LED Strip | 60 LEDs/m, IP65 | 2m | $15-25 | Multiple | WS2812B-60 |
| Push Button | 12mm, tactile | 3 | $0.50 each | Digikey | B3F-1000 |
| Pull-up Resistor | 10kΩ, 1/4W | 3 | $0.05 each | Digikey | CF14JT10K0 |
| Power Capacitor | 1000µF, 16V | 1 | $1-2 | Digikey | ECA-1CM102 |

### Power System
| Part | Specification | Quantity | Unit Cost | Supplier | Part Number |
|------|---------------|----------|-----------|----------|-------------|
| Buck Converter | 12V→5V, 3A | 1 | $8-12 | Multiple | LM2596-ADJ |
| Fuse Holder | 5A, panel mount | 1 | $2-3 | Digikey | 36-3035-ND |
| Fuses | 5A slow-blow | 3 | $0.50 each | Digikey | BK/AGC-5 |
| Power Switch | SPST, 10A | 1 | $3-5 | Digikey | EG1218-ND |
| Battery Connector | Anderson Powerpole | 1 set | $3-5 | Multiple | PP15/45 |

### LED Strips (Detailed)
| Component | LEDs | Length | Purpose | Color | Cost |
|-----------|------|--------|---------|-------|------|
| Left Turn Strip | 20 | 33cm | Turn signal | Amber | $8-12 |
| Right Turn Strip | 20 | 33cm | Turn signal | Amber | $8-12 |
| Brake Strip | 30 | 50cm | Brake light | Red | $10-15 |

*Note: Using programmable LEDs allows color changes in software*

### Enclosure & Protection
| Part | Specification | Quantity | Unit Cost | Purpose |
|------|---------------|----------|-----------|---------|
| Main Enclosure | IP67, 150x100x50mm | 1 | $15-25 | Controller housing |
| LED Mounting | Aluminum channel | 3m | $8-12 | Heat dissipation |
| Wire Protector | Split loom, 10mm | 5m | $5-8 | Cable protection |
| Connectors | IP65, 3-pin | 6 | $2-3 each | LED connections |
| Sealant | Silicone, marine grade | 1 tube | $3-5 | Weatherproofing |

## 🔋 Power & Battery Options

### BLE Sniffer Power
| Option | Capacity | Runtime | Cost | Notes |
|--------|----------|---------|------|-------|
| USB Power Bank | 10,000mAh | 20+ hours | $15-25 | Easiest option |
| 18650 Battery Pack | 3,000mAh | 8-12 hours | $10-15 | Compact |
| Car 12V Adapter | Unlimited | Continuous | $5-10 | Vehicle use |

### LED Controller Power
| Option | Capacity | Runtime | Cost | Notes |
|--------|----------|---------|------|-------|
| 12V SLA Battery | 7Ah | 4-6 hours | $25-35 | Reliable |
| LiFePO4 Battery | 5Ah | 5-8 hours | $40-60 | Lightweight |
| Car Battery | 60Ah+ | 24+ hours | $80-120 | Maximum runtime |

## 🧰 Tools & Supplies

### Essential Tools
| Tool | Specification | Cost | Purpose |
|------|---------------|------|---------|
| Soldering Iron | Temperature controlled, 40W | $25-50 | Assembly |
| Solder | 60/40 or lead-free, 0.8mm | $5-10 | Assembly |
| Wire Strippers | 20-30 AWG | $10-15 | Wire preparation |
| Multimeter | Basic digital | $15-30 | Testing |
| Heat Gun | Variable temperature | $20-40 | Heat shrink |

### Consumables
| Item | Specification | Cost | Usage |
|------|---------------|------|-------|
| Hook-up Wire | 22 AWG, stranded | $10-15 | Connections |
| Heat Shrink Tubing | Assorted sizes | $5-10 | Protection |
| Cable Ties | UV resistant | $3-5 | Cable management |
| Flux | Rosin core | $3-5 | Soldering aid |
| Isopropyl Alcohol | 99% | $3-5 | Cleaning |

## 🛡️ Safety & Protection

### Circuit Protection
| Component | Rating | Quantity | Cost | Purpose |
|-----------|--------|----------|------|---------|
| TVS Diodes | 15V, 1500W | 2 | $1-2 each | ESD protection |
| Schottky Diode | 5A, 40V | 1 | $1-2 | Reverse polarity |
| Ferrite Beads | 1000Ω@100MHz | 5 | $0.25 each | EMI suppression |

### Personal Safety
| Item | Cost | Purpose |
|------|------|---------|
| Safety Glasses | $5-10 | Eye protection |
| Anti-static Wrist Strap | $5-8 | ESD protection |
| Ventilation Fan | $15-25 | Fume extraction |
| First Aid Kit | $10-20 | Emergency care |

## 📦 Supplier Information

### Primary Suppliers (US)

**Electronic Components:**
- **Digikey** (digikey.com)
  - Pros: Vast selection, reliable, fast shipping
  - Cons: Higher prices for small quantities
  - Best for: Precision components, datasheets

- **Mouser** (mouser.com)
  - Pros: Good selection, technical support
  - Cons: Similar pricing to Digikey
  - Best for: Professional components

- **Amazon** (amazon.com)
  - Pros: Fast shipping, easy returns
  - Cons: Quality varies, counterfeit parts
  - Best for: Development boards, basic parts

**LED Components:**
- **Adafruit** (adafruit.com)
  - Pros: High quality, excellent documentation
  - Cons: Higher prices
  - Best for: NeoPixel strips, reliable parts

- **AliExpress** (aliexpress.com)
  - Pros: Low cost, bulk quantities
  - Cons: Long shipping, quality varies
  - Best for: LED strips, enclosures

**Power Components:**
- **Battery Space** (batteryspace.com)
  - Pros: Specialized in batteries
  - Best for: Custom battery packs

- **All Electronics** (allelectronics.com)
  - Pros: Surplus components, low cost
  - Best for: Enclosures, mechanical parts

### International Options

**Europe:**
- **RS Components** (rs-online.com)
- **Farnell** (farnell.com)
- **Conrad** (conrad.com)

**Asia:**
- **LCSC** (lcsc.com) - China
- **Element14** (element14.com) - Global
- **Taobao** (taobao.com) - China (requires agent)

## 📋 Alternative Parts

### ESP32 Alternatives
| Part | Pros | Cons | Cost Difference |
|------|------|------|-----------------|
| ESP32-S3 | More powerful, USB-C | Higher cost | +$3-5 |
| ESP32-C3 | Lower cost, RISC-V | No dual-core | -$2-3 |
| Arduino Nano 33 BLE | Arduino ecosystem | More expensive | +$15-20 |

### LED Strip Alternatives
| Type | Pros | Cons | Cost Impact |
|------|------|------|-------------|
| SK6812 | RGBW, better colors | Slightly more expensive | +10% |
| APA102 | Higher refresh rate | More complex wiring | +20% |
| Standard LEDs | Lower cost | No programmability | -50% |

### Power Alternatives
| Option | Pros | Cons | Cost Impact |
|--------|------|------|-------------|
| Linear Regulator | Simple, cheap | Inefficient, heat | -$5 |
| Integrated Buck | Compact | Less flexible | +$2-3 |
| Switching Module | High efficiency | More complex | +$3-5 |

## 🔍 Quality Considerations

### Component Grades
| Grade | Quality Level | Cost Impact | Recommended For |
|-------|---------------|-------------|-----------------|
| Industrial | High reliability | +50-100% | Critical systems |
| Commercial | Standard quality | Baseline | General use |
| Consumer | Basic quality | -20-30% | Non-critical applications |

### Reliability Factors
- **Temperature rating**: Choose -40°C to +85°C for outdoor use
- **Voltage tolerance**: ±10% minimum for automotive environments
- **Vibration rating**: Important for bicycle applications
- **Moisture resistance**: IP65+ for outdoor electronics

## 💡 Cost Optimization Tips

### Bulk Purchasing
- Order multiple units: 10-20% savings
- Group buys with community: Additional savings
- Annual component purchases: Better supplier terms

### Alternative Sourcing
- Surplus electronics stores: 30-50% savings
- Hobbyist forums: Used components
- Educational discounts: Student pricing

### Design Choices
- Use standard form factors: Lower cost
- Minimize custom PCBs: Use perfboard/breadboard
- Shared components: Same part for multiple functions

## 📊 Build Complexity Matrix

| Component | Difficulty | Time | Special Tools |
|-----------|------------|------|---------------|
| BLE Sniffer | Easy | 2 hours | Basic soldering |
| LED Controller | Medium | 4 hours | Multimeter |
| Power System | Medium | 2 hours | Load testing |
| Enclosures | Easy | 1 hour | Drill, files |
| **Total Project** | **Medium** | **9 hours** | **Standard tools** |

### Skill Requirements
- **Beginner**: Can build BLE sniffer only
- **Intermediate**: Complete system possible
- **Advanced**: Custom modifications and PCB design

## 🔄 Upgrade Path

### Phase 1 (Basic): $50-75
- Single ESP32 with basic BLE scanning
- Simple LED indicators
- Breadboard construction

### Phase 2 (Functional): $100-150
- Complete dual-unit system
- Basic LED strips
- Simple enclosures

### Phase 3 (Professional): $200-300
- Custom PCBs
- High-quality enclosures
- Advanced features (GPS, cellular)

---

This parts list provides multiple options and price points to suit different budgets and skill levels. Start with the basic version and upgrade components as needed.