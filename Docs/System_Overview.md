# System Overview

This project implements a smart PowerBank system using a PIC16F688 microcontroller.

The system is designed to manage:
- Battery monitoring for a 3-cell Li-ion pack
- Load control using a relay
- Charging control with overvoltage protection
- User interface using a single button
- Flashlight functionality
- Power management (Sleep / Wake system)

## Main Functionalities

### 1. Battery Monitoring
- Reads 3 battery taps using ADC
- Calculates individual cell voltages
- Protects against undervoltage (cutoff)

### 2. Load Control
- Controls a relay to enable/disable output
- Automatically turns OFF if any cell is below threshold

### 3. Charging System
- Detects charger connection
- Controls charging using a dedicated relay
- Stops charging if any cell reaches 4.25V
- Uses latch mechanism to prevent unstable behavior

### 4. User Interface (Button)
- Short press → Show battery level
- Long press → Toggle flashlight
- Double click → Sleep / Wake system

### 5. Flashlight
- Controlled by long press
- Automatically disabled when system is off or unsafe

### 6. Power Management
- Sleep mode reduces system activity
- Wake via double click or charger connection
- Startup indication using LEDs (3 seconds)

## Design Goal

The main goal of this system is to simulate a **real-world PowerBank controller** with:
- Safety
- Stability
- Clean user experience
- Modular embedded architecture