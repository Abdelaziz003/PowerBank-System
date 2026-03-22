# Smart Power Bank Control System

A smart embedded power bank controller built using **PIC16F688**.  
The system monitors battery tap voltages, estimates battery level, protects the load using a relay, detects charger connection and charging current, and controls a flashlight LED using PWM through a MOSFET.

## Highlights

- 4-level battery indicator:
  - 25%
  - 50%
  - 75%
  - 100%
- Continuous battery monitoring
- Relay-based undervoltage protection
- Charger connection detection
- Shunt-based charging current detection
- Automatic charging/full indication
- Flashlight LED with long-press control
- PWM-based flashlight driving
- Timer-based firmware architecture

## How It Works

### Battery Level
- **Short press** shows battery level when charger is not connected.
- When charger is connected, battery level is displayed automatically.

### Charging Behavior
- **Charger connected + charging current present**
  - Normal battery level display
  - Last active LED blinks
- **Charger connected + no charging current**
  - All 4 LEDs turn ON
  - Battery considered fully charged

### Protection
- Battery taps are monitored continuously.
- Cell voltages are derived from cumulative tap readings.
- If any cell falls below the critical threshold:
  - Load relay is turned OFF
  - Flashlight is forced OFF

### Flashlight
- Controlled by **long press**
- Driven through a MOSFET using PWM
- Disabled automatically when protection is active

## Simulation Approach

In Proteus, the battery taps are represented using **interactive variable voltage emulators** instead of real series battery cells.

This makes it possible to:
- change tap voltages during runtime
- test undervoltage conditions quickly
- validate ADC logic interactively
- demonstrate charging/full-state behavior easily

A separate shunt-based test section is used to emulate charging current behavior.

## Firmware Architecture

- **MCAL**
  - DIO
  - ADC
  - TIMER0
- **HAL**
  - BUTTON
- **APP**
  - BATTERY_MONITOR
  - BATTERY_LEVEL
  - CHARGER_DETECT
  - CHARGE_CURRENT_DETECT
  - FLASHLIGHT
  - POWERBANK_CTRL

## Tools

- MPLAB X IDE
- XC8 Compiler
- Proteus
- Embedded C

## Project Structure

```text
PowerBank-System/
├── README.md
├── Firmware/
├── Hardware/
└── Docs/