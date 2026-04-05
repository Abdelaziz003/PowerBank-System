# PowerBank System (Embedded Project)

This project implements a smart PowerBank system using PIC16F688.

The system simulates a real-world power bank controller with advanced features and protection mechanisms.

## Features

- Battery monitoring (3 Li-ion cells)
- Load control using relay
- Flashlight control
- User interface with single button
- Sleep / Wake system using double-click
- Smart charging system with overvoltage protection

## User Interface

- Short press → Battery level
- Long press → Flashlight
- Double click → Sleep / Wake

## Charging System

- Charging starts when charger is connected
- Stops if any cell reaches 4.25V
- Uses latch mechanism:
  - No restart until charger is disconnected

## Architecture

- LIB / MCAL / HAL / APP layered design
- Modular drivers
- Clean separation of logic

## Hardware

- PIC16F688
- Relays (Load + Charge)
- Voltage dividers
- Proteus simulation included

## Author
Eng. Abdelaziz M. S. Lubad.