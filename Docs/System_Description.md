# System Description

## 1. Introduction
The Smart Power Bank Control System is a PIC16F688-based embedded application that manages battery monitoring, user indication, protection logic, charger handling, charge-current detection, and flashlight control.

The design is intended as a practical embedded control platform combining:
- ADC-based sensing
- digital outputs
- timer-based scheduling
- layered firmware
- Proteus-based interactive simulation

---

## 2. Functional Overview
The system performs the following functions:

1. Measures three battery tap voltages
2. Derives individual cell voltages
3. Determines battery level indication
4. Detects charger connection
5. Detects actual charging current through a shunt resistor
6. Controls a protection relay
7. Drives a flashlight LED using PWM
8. Handles user input using short press and long press actions

---

## 3. Hardware Blocks

### 3.1 Microcontroller
- **Device:** PIC16F688
- Used peripherals:
  - GPIO
  - ADC
  - Timer0

### 3.2 Tap Voltage Inputs
Three analog measurement paths are used:
- Tap1
- Tap2
- Tap3

Each tap is connected to the MCU through:
- resistor divider
- input filtering capacitor

### 3.3 Battery Level LEDs
Four LEDs indicate:
- 25%
- 50%
- 75%
- 100%

### 3.4 User Button
One push button is used for:
- short press: battery level display
- long press: flashlight toggle

### 3.5 Protection Relay
A transistor-driven relay disconnects the load under undervoltage conditions.

### 3.6 Charger Detection
A dedicated charger detect input indicates whether a charger is connected.

### 3.7 Current Sense Section
A shunt resistor is used to detect whether charge current is present.

### 3.8 Flashlight Stage
A MOSFET-based output stage is used to drive the flashlight LED safely.

---

## 4. Simulation Method

### 4.1 Tap Emulation
In simulation, real battery cells are not modeled directly as a series electrochemical pack.  
Instead, the battery taps are emulated using adjustable voltage sources.

This allows:
- live voltage changes
- repeatable test cases
- simple demonstration of firmware reactions

### 4.2 Current Sense Test Circuit
A dedicated shunt-based charging test arrangement is used in simulation to emulate the presence or absence of charging current.

This allows the firmware to validate:
- charging state
- fully charged state
- charger connected without current

---

## 5. Voltage Processing

### 5.1 Tap Measurements
The firmware reads cumulative voltages:
- `Tap1`
- `Tap2`
- `Tap3`

### 5.2 Cell Voltage Derivation
The firmware derives:
- `Cell1 = Tap1`
- `Cell2 = Tap2 - Tap1`
- `Cell3 = Tap3 - Tap2`

### 5.3 Low-Side Current Sense Correction
When low-side shunt sensing is enabled, the battery negative reference shifts above ground during charging.

Therefore, the firmware compensates by subtracting the current-sense offset from:
- Tap1
- Tap2
- Tap3

before calculating individual cells.

---

## 6. Protection Logic

### 6.1 Disconnect Condition
If any cell voltage falls below the disconnect threshold:
- relay is turned OFF

### 6.2 Reconnect Condition
If all cells rise above the reconnect threshold:
- relay may be turned ON again

### 6.3 Filtering
The relay decision includes filtering counters to avoid unstable switching due to noise or temporary fluctuations.

---

## 7. User Interface Logic

### 7.1 Battery Level Display
The battery level display has two operating modes.

#### Manual Display Mode
Used when charger is not connected:
- a short press displays battery level temporarily

#### Automatic Display Mode
Used when charger is connected:
- battery level display remains active automatically

### 7.2 Charging Indication
If charger is connected and charging current exists:
- the battery level is shown
- the last active LED blinks

### 7.3 Full Indication
If charger is connected and charging current does not exist:
- all battery LEDs turn ON steadily

---

## 8. Flashlight Logic

### 8.1 User Control
- long press toggles flashlight ON/OFF

### 8.2 PWM Control
The flashlight output uses PWM to reduce average current and heat.

### 8.3 Safety Interlock
If the relay enters protection state:
- flashlight is forced OFF

This ensures that protection has priority over the user flashlight request.

---

## 9. Timing Model

### 9.1 Timer Source
Timer0 is used as the time base.

### 9.2 Scheduling Strategy
A fast periodic timing base is used, then software counters divide this base into:
- flashlight PWM timing
- main controller timing

This allows:
- accurate long-press timing
- accurate display timing
- stable blink timing
- smoother flashlight PWM

---

## 10. Software Structure

### 10.1 MCAL Layer
Provides low-level access to microcontroller peripherals:
- DIO
- ADC
- TIMER0

### 10.2 HAL Layer
Provides reusable hardware abstraction:
- BUTTON

### 10.3 APP Layer
Implements system behavior:
- BATTERY_MONITOR
- BATTERY_LEVEL
- CHARGER_DETECT
- CHARGE_CURRENT_DETECT
- FLASHLIGHT
- POWERBANK_CTRL

---

## 11. Main Operating States

### 11.1 Charger Disconnected
- no automatic display
- short press displays battery level
- long press controls flashlight
- relay remains controlled by battery protection logic

### 11.2 Charger Connected + Charging Current Present
- automatic display enabled
- last active battery LED blinks
- flashlight may still operate if relay is ON

### 11.3 Charger Connected + No Charging Current
- automatic display enabled
- all 4 LEDs ON
- battery considered fully charged

### 11.4 Protection State
- relay OFF
- flashlight OFF
- protection has highest priority

---

## 12. Design Intent
This project is intentionally designed as an embedded control and monitoring platform rather than a chemistry-accurate battery simulator.

Its main goals are:
- real-time sensing
- decision making
- protection
- user indication
- modular firmware design
- practical educational value

---

## 13. Conclusion
The system demonstrates how a low-cost 8-bit microcontroller can be used to implement a structured and feature-rich power bank controller including:
- battery monitoring
- load protection
- charger-aware behavior
- current-based full-charge indication
- PWM flashlight control
- timer-based firmware scheduling

The project can be extended further with:
- UART debugging
- ADC calibration
- EEPROM settings
- more advanced scheduling
- refined current and battery estimation
