# Charging Logic

The charging system is implemented using a dedicated module: `CHARGE_CTRL`.

## Goal

To provide:
- Safe charging
- Overvoltage protection
- Stable behavior without relay oscillation

## Key Concept: Charging Session Latch

The system introduces a concept called:

> "Charge Cutoff Latch per Charger Session"

## Behavior

### When charger is connected
- A new charging session starts
- Charging relay is enabled (if safe)

### During charging
- System continuously monitors all cells

### If any cell ≥ 4.25V
- Charging relay is turned OFF
- A latch is activated
- Charging is permanently stopped for this session

### Important:
Even if voltage drops later:
- Charging will NOT resume
- Until charger is disconnected

### When charger is disconnected
- Latch is cleared
- System resets charging state

### When charger is reconnected
- New session starts
- Charging allowed again if voltages are safe

## LED Behavior

| State | LEDs |
|------|------|
| Charging active | Level + blinking |
| Charging cutoff (latched) | All LEDs ON |
| No charger | Normal behavior |

## Why this approach?

Compared to current detection:
- More reliable
- Direct voltage-based protection
- Prevents relay oscillation
- Closer to real BMS behavior