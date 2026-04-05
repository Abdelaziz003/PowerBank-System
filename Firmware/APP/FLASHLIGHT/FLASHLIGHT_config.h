#ifndef FLASHLIGHT_CONFIG_H
#define FLASHLIGHT_CONFIG_H

#include "../../MCAL/DIO/DIO_interface.h"

/*
 * RC4 now controls 555 RESET pin.
 * HIGH -> 555 enabled -> flashlight ON
 * LOW  -> 555 disabled -> flashlight OFF
 */
#define FLASH_OUT_PORT              DIO_PORTC
#define FLASH_OUT_PIN               DIO_PIN4

#endif