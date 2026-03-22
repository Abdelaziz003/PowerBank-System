#ifndef POWERBANK_CTRL_CONFIG_H
#define POWERBANK_CTRL_CONFIG_H

/*
 * Battery level display duration after short press release.
 * Increase this value to keep battery LEDs ON for a longer time.
 * Decrease it to make the display turn OFF faster.
 *
 * With ~10 ms system tick:
 * 100 ticks ~= 1 second
 */
#define PBCTRL_DISPLAY_TICKS_AFTER_SHORT_PRESS   100U

/*
 * Blink speed divider for charging indication.
 * Increase this value to make blinking slower.
 * Decrease it to make blinking faster.
 *
 * With ~10 ms system tick:
 * 15 ticks ~= 150 ms per toggle
 */
#define PBCTRL_BLINK_COUNTER_LIMIT               50U

#endif