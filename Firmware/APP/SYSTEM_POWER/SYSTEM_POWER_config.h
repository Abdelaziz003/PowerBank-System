#ifndef SYSTEM_POWER_CONFIG_H
#define SYSTEM_POWER_CONFIG_H

/*
 * All values are expressed in PBCTRL update ticks.
 * Assumption:
 * 1 PBCTRL update tick ~= 10 ms
 */

/*
 * Startup indication duration = 3 seconds.
 * 3 s / 10 ms = 300 ticks
 */
#define SPWR_STARTUP_DURATION_TICKS     300U

#endif