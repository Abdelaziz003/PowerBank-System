# Configuration Files

This file explains the configurable parameters in the system.

## 1. PowerBank Control Config

File: `POWERBANK_CTRL_config.h`

- PBCTRL_DOUBLE_CLICK_WINDOW_TICKS
  - Controls double-click detection time

- PBCTRL_DISPLAY_TICKS_AFTER_SHORT_PRESS
  - Controls battery level display duration

## 2. Charge Control Config

File: `CHARGE_CTRL_config.h`

- CHCTRL_CELL_CUTOFF_MV
  - Overvoltage threshold (default: 4250 mV)

- Relay pin configuration
  - Define port and pin for charge relay

## 3. Battery Monitor Config

- Voltage divider values
- ADC reference voltage

## 4. System Power Config

- Startup duration (3 seconds)

## Note

All timing values depend on:
- System tick = 1ms
- PBCTRL update ≈ 10ms