# Testing Guide

This guide explains how to test the system step by step.

## 1. Basic Functionality

- Power ON system
- Check LEDs are OFF initially
- Press button → battery level should appear

## 2. Flashlight

- Long press → flashlight ON
- Long press again → flashlight OFF

## 3. Sleep / Wake

- Double click → system enters sleep
- Double click again → system wakes
- Startup LEDs should light for ~3 seconds

## 4. Load Protection

- Reduce any cell voltage below threshold
- Relay should turn OFF
- Load should disconnect

## 5. Charging Test

### Case 1: Normal Charging
- Connect charger
- Charging relay should turn ON
- LEDs should blink (charging indication)

### Case 2: Overvoltage Cutoff
- Increase any cell above 4.25V
- Charging relay should turn OFF
- All LEDs should turn ON

### Case 3: Latch Behavior
- Keep charger connected
- Reduce voltage below 4.25V
- Charging must NOT resume

### Case 4: New Session
- Disconnect charger
- Reconnect charger
- Charging should start again

## 6. Edge Cases

- Charger connected during sleep → system should wake
- Flashlight should not work if relay is OFF