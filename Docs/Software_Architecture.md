# Software Architecture

The system is designed using a layered architecture commonly used in embedded systems.

## Layers

### 1. LIB Layer
Basic utilities:
- Standard types
- Bit manipulation

### 2. MCAL (Microcontroller Abstraction Layer)
Direct interaction with hardware:
- DIO (Digital I/O)
- ADC (Analog to Digital Converter)
- TIMER0 (system tick)

### 3. HAL (Hardware Abstraction Layer)
Drivers for external hardware:
- BUTTON driver

### 4. APP Layer (Application)
This is the main logic of the system.

## APP Modules

### POWERBANK_CTRL
- Main controller of the system
- Handles button logic
- Controls system behavior

### SYSTEM_POWER
- Manages system states:
  - ACTIVE
  - SLEEP
  - STARTUP

### BATTERY_MONITOR
- Reads battery voltages
- Calculates cell voltages
- Controls load relay

### BATTERY_LEVEL
- Displays battery percentage using LEDs

### FLASHLIGHT
- Controls high-power LED

### CHARGER_DETECT
- Detects if charger is connected

### CHARGE_CTRL (New Module)
- Controls charging relay
- Implements overcharge protection
- Uses latch mechanism per charging session

## Architecture Advantages

- Modular design
- Easy to debug and extend
- Clear separation of responsibilities
- Scalable for future features