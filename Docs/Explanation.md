# Explanation

## Project Purpose
This project implements a smart power bank control system using the **PIC16F688** microcontroller.  
The firmware monitors battery tap voltages, estimates battery level, protects the load through a relay, detects charger connection, detects charging current using a shunt resistor, and controls a flashlight LED using PWM through a MOSFET.

The project was designed for:
- embedded systems practice
- ADC-based battery monitoring
- protection and control logic
- real-time event handling
- simulation and demonstration in Proteus

---

## Main Idea
The system continuously reads battery-related analog inputs and makes decisions based on:
- battery tap voltages
- charger connection state
- charge current existence
- user button actions

From these inputs, the controller decides:
- whether the load relay should remain ON or OFF
- what battery level should be displayed
- whether charging is active or complete
- whether the flashlight LED may operate

---

## Battery Voltage Measurement
The battery pack is treated as a 3-cell series pack.  
Instead of reading each cell directly with a dedicated isolated measurement chain, the firmware reads:

- **Tap1**
- **Tap2**
- **Tap3**

Then the software derives:
- `Cell1 = Tap1`
- `Cell2 = Tap2 - Tap1`
- `Cell3 = Tap3 - Tap2`

This allows the system to estimate the individual cell voltages using cumulative tap measurements.

---

## Why Tap Emulators Were Used in Simulation
In Proteus, the battery taps are emulated using **interactive variable voltage sources** instead of real electrochemical battery models.

This choice was intentional because it allows:
- changing voltages during simulation runtime
- testing undervoltage and recovery conditions quickly
- demonstrating battery level transitions interactively
- validating ADC and firmware behavior in a controlled way

So the simulation should be understood as a **tap-voltage emulator testbench**, not as a detailed electrochemical battery model.

---

## Charger Detection
The firmware includes charger connection detection logic.  
When the charger is detected, the user interface behavior changes:

- battery level is shown automatically
- the charging indication becomes active
- the last active battery LED blinks while charging current exists

When the charger is removed:
- the automatic display is cleared
- the system returns to manual battery-level display through the button

---

## Charge Current Detection
A shunt-based sensing method is used to determine whether charging current is actually flowing.

This is useful because the system distinguishes between:
- **charger connected and charging**
- **charger connected but no charge current**
- **charger disconnected**

When:
- charger is connected
- and charging current is present

the battery level is displayed normally and the last active LED blinks.

When:
- charger is connected
- but no charging current is detected

the system interprets this as the battery being fully charged and turns ON all 4 battery level LEDs steadily.

---

## Low-Side Shunt Compensation
Because the shunt is used in the low-side current path, the battery negative terminal may be shifted slightly above system ground during charging.

This means that the measured tap voltages can become higher than their real values by the amount of the shunt voltage.

To solve this, the firmware subtracts the current-sense offset from all tap readings whenever current-sense compensation is enabled.  
This keeps:
- pack voltage calculation correct
- cell voltage calculation correct
- relay decision logic stable

---

## Relay-Based Protection
The relay is used to disconnect the load when the battery reaches a dangerous undervoltage condition.

The firmware continuously checks the derived cell voltages.  
If any cell falls below the critical disconnect threshold:
- the relay turns OFF
- the load is disconnected

When all cells recover above the reconnect threshold:
- the relay may return to ON

This logic includes filtering and hysteresis to prevent unstable switching.

---

## Battery Level Indication
The battery level is shown using 4 LEDs:
- 25%
- 50%
- 75%
- 100%

The battery level display behavior depends on operating mode.

### Manual Mode
When charger is not connected:
- short press displays battery level temporarily

### Charging Mode
When charger is connected:
- battery level is displayed automatically
- if charging current exists, the last active LED blinks
- if no charging current exists, all LEDs turn ON steadily

---

## Flashlight LED
The flashlight is controlled by long press.

The flashlight output is not driven directly from the PIC pin.  
Instead, the microcontroller drives a MOSFET, and the MOSFET drives the LED load.

This was chosen because:
- the flashlight LED can require higher current
- PWM operation is needed
- the MCU pin must only provide gate drive, not load current

---

## PWM Flashlight Control
The flashlight uses software PWM to reduce:
- average current consumption
- LED heating
- battery stress

The LED is not supplied with continuous current.  
Instead, it receives pulses:
- ON for part of the period
- OFF for the rest of the period

The PWM duty cycle controls the average brightness and average current.

---

## Flashlight Safety Rule
If battery protection disables the relay:
- the flashlight is forced OFF

This ensures that the protection state has priority over user requests.

Even if the flashlight had been enabled earlier, it is shut down immediately when the relay enters the protection state.

---

## Timer-Based Scheduling
The firmware uses **Timer0** as the system timing source.

The design separates tasks logically:
- a faster timing base is used for flashlight PWM updates
- a slower periodic controller update is used for the main system behavior

This improves:
- long-press stability
- battery display timing
- charging blink timing
- flashlight PWM smoothness

---

## Firmware Organization
The code is organized into layers.

### MCAL
Low-level hardware access:
- DIO
- ADC
- TIMER0

### HAL
Reusable hardware abstraction:
- BUTTON

### APP
Application behavior:
- BATTERY_MONITOR
- BATTERY_LEVEL
- CHARGER_DETECT
- CHARGE_CURRENT_DETECT
- FLASHLIGHT
- POWERBANK_CTRL

This makes the project easier to:
- debug
- extend
- maintain
- demonstrate

---

## Project Value
This project combines several important embedded concepts in one system:
- analog measurement
- derived calculation from cumulative taps
- hysteresis-based protection
- PWM output control
- event-driven button handling
- timer-based scheduling
- structured embedded software design

It is suitable as:
- an educational project
- a portfolio project
- a practical embedded systems reference
