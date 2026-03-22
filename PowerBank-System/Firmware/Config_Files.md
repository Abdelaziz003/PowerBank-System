# Config Files

This document describes the main configuration files used in the firmware and what each one controls.

## Overview
The firmware uses dedicated `config.h` files to centralize tunable parameters.  
This makes the project easier to:
- calibrate
- tune
- maintain
- port
- demonstrate

---

## 1. `MCAL/ADC/ADC_config.h`
Controls ADC peripheral behavior.

Typical settings may include:
- ADC clock selection
- acquisition configuration
- result formatting
- enabled analog channels

### Why it matters
This file defines how analog inputs are sampled and converted.

---

## 2. `MCAL/TIMER0/TIMER0_config.h`
Controls Timer0 timing behavior.

Typical settings include:
- timer or counter mode
- prescaler assignment
- prescaler ratio
- edge selection in counter mode

### Why it matters
This file defines the system timing base used by the project.

---

## 3. `HAL/BUTTON/BUTTON_config.h`
Controls button timing and button electrical configuration.

Typical settings include:
- button port/pin
- active level
- debounce count
- long press threshold

### Why it matters
This file defines how fast the button responds and how long the user must press for a long press event.

---

## 4. `APP/BATTERY_MONITOR/BATTERY_MONITOR_config.h`
Controls battery monitoring behavior.

Typical settings include:
- ADC channels for Tap1, Tap2, Tap3
- current-sense channel
- relay output pin
- ADC reference voltage
- cell disconnect threshold
- cell reconnect threshold
- divider resistor values
- current-sense divider values
- filtering counters

### Why it matters
This is one of the most important configuration files in the project because it directly affects:
- measured voltage correctness
- relay behavior
- protection thresholds

---

## 5. `APP/BATTERY_LEVEL/BATTERY_LEVEL_config.h`
Controls battery level indication mapping.

Typical settings include:
- LED pins for 25%, 50%, 75%, 100%
- battery level thresholds

### Why it matters
This file determines how measured battery voltage is translated into visible user indication.

---

## 6. `APP/FLASHLIGHT/FLASHLIGHT_config.h`
Controls flashlight output configuration and PWM behavior.

Typical settings include:
- output pin
- PWM period ticks
- PWM ON ticks

### Why it matters
This file affects:
- flashlight brightness
- average current
- average heat
- PWM smoothness

---

## 7. `APP/CHARGER_DETECT/CHARGER_DETECT_config.h`
Controls charger detection input settings.

Typical settings include:
- input port/pin
- active level

### Why it matters
This file determines how the firmware interprets charger connection.

---

## 8. `APP/CHARGE_CURRENT_DETECT/CHARGE_CURRENT_DETECT_config.h`
Controls current-sense-based charging detection.

Typical settings include:
- feature enable/disable switch
- ADC channel used for current sense
- ADC reference
- divider settings
- current-present threshold
- current-absent threshold

### Why it matters
This file determines when the firmware decides:
- charging is active
- charging has stopped
- battery is considered fully charged

---

## 9. `APP/POWERBANK_CTRL/POWERBANK_CTRL_config.h`
Controls high-level user behavior timing.

Typical settings include:
- battery level display duration
- blink timing divider

### Why it matters
This file controls user experience:
- how long battery level stays visible
- how fast the charging blink appears

---

## Recommended Tuning Strategy

When tuning the project, the safest order is:

1. ADC and analog channels
2. Battery monitor divider values
3. Battery thresholds
4. Button timing
5. Flashlight PWM timing
6. Charger/current-detect thresholds
7. UI blink/display timing

---

## Important Note
For best maintenance, any value that may need future adjustment should remain in a `config.h` file rather than being hardcoded inside program logic.

This includes:
- thresholds
- timing values
- pins
- channels
- PWM ratios
- feature enable switches
