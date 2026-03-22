#ifndef FLASHLIGHT_CONFIG_H
#define FLASHLIGHT_CONFIG_H

#include "../../MCAL/DIO/DIO_interface.h"

/* Output pin that drives MOSFET / transistor */
#define FLASH_OUT_PORT              DIO_PORTC
#define FLASH_OUT_PIN               DIO_PIN4

/*
 * Total software PWM period in 1 ms ticks.
 * Increase this value to make PWM cycle longer.
 * Decrease it to make PWM cycle shorter.
 *
 * Example:
 * 5 ticks = 5 ms period -> about 200 Hz
 */
#define FLASH_PWM_PERIOD_TICKS      5U

/*
 * ON-time inside the PWM period in 1 ms ticks.
 * Increase this value to raise average brightness and current.
 * Decrease it to reduce brightness, current consumption, and heat.
 *
 * This value must always be <= FLASH_PWM_PERIOD_TICKS.
 *
 * Example:
 * 2 / 5 = 40% duty cycle
 */
#define FLASH_PWM_ON_TICKS          2U

#endif