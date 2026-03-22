#ifndef BUTTON_CONFIG_H
#define BUTTON_CONFIG_H

#define BUTTON_PORT                DIO_PORTA
#define BUTTON_PIN                 DIO_PIN3

#define BUTTON_ACTIVE_MODE         BUTTON_ACTIVE_LOW

/*
 * Debounce filter length in update ticks.
 * Increase this value to make button filtering stronger but slower.
 * Decrease it to make button response faster but more sensitive to bounce.
 *
 * With ~10 ms system tick:
 * 3 ticks ~= 30 ms
 */
#define BUTTON_DEBOUNCE_COUNT      3

/*
 * Long press threshold in update ticks.
 * Increase this value to require a longer button hold.
 * Decrease it to make long press trigger faster.
 *
 * With ~10 ms system tick:
 * 100 ticks ~= 1 second
 */
#define BUTTON_LONG_PRESS_TICKS    50

#endif