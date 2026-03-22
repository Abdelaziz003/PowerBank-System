#ifndef TIMER0_CONFIG_H
#define TIMER0_CONFIG_H

#include "TIMER0_interface.h"

/*
 * Timer0 operating mode.
 * TIMER mode uses internal clock for time base.
 * COUNTER mode uses external pulses.
 */
#define TIMER0_MODE_CONFIG              TIMER0_TIMER_MODE

/*
 * Prescaler assignment.
 * Keep it assigned to Timer0 when using Timer0 for periodic timing.
 */
#define TIMER0_PRESCALER_ASSIGN_CONFIG  TIMER0_PRESCALER_TO_TIMER0

/*
 * Timer0 prescaler value.
 * Increase prescaler to make Timer0 overflow slower.
 * Decrease prescaler to make Timer0 overflow faster.
 *
 * 000 -> 1:2
 * 001 -> 1:4
 * 010 -> 1:8
 * 011 -> 1:16
 * 100 -> 1:32
 * 101 -> 1:64
 * 110 -> 1:128
 * 111 -> 1:256
 */

/*
 * Prescaler = 1:32
 * Used with preload 193 to generate about 1 ms Timer0 tick at 8 MHz oscillator.
 * Increase prescaler to make Timer0 overflow slower.
 * Decrease prescaler to make Timer0 overflow faster.
 */
#define TIMER0_PRESCALER_VALUE_CONFIG   0b100

/*
 * Counter edge selection.
 * Used only if TIMER0 is configured in COUNTER mode.
 */
#define TIMER0_COUNTER_EDGE_CONFIG      TIMER0_TRANSITION_LOW_TO_HIGH

#endif