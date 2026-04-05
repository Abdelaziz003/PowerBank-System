#ifndef POWERBANK_CTRL_CONFIG_H
#define POWERBANK_CTRL_CONFIG_H

/*
 * Battery level display duration after single short press.
 * Assumption:
 * 1 PBCTRL update tick ~= 10 ms
 */
#define PBCTRL_DISPLAY_TICKS_AFTER_SHORT_PRESS   100U   /* 1 second */

/*
 * Blink speed divider for charging indication.
 * 15 ticks ~= 150 ms per toggle when PBCTRL update is 10 ms
 */
#define PBCTRL_BLINK_COUNTER_LIMIT               15U

/*
 * Double-click detection window.
 * A single short press is delayed for this window so the firmware
 * can decide whether the user intended:
 * - single click  -> battery level
 * - double click  -> sleep / wake
 *
 * 30 ticks ~= 300 ms
 */
#define PBCTRL_DOUBLE_CLICK_WINDOW_TICKS         40U

#endif