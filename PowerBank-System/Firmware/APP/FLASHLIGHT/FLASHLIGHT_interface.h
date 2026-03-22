#ifndef FLASHLIGHT_INTERFACE_H
#define FLASHLIGHT_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

/* Return status */
#define FLASH_OK     0
#define FLASH_NOK    1

/* Flashlight states */
#define FLASH_OFF    0
#define FLASH_ON     1

/*
 * Function: FLASH_u8Init
 * ----------------------
 * Initializes flashlight output pin and internal state.
 */
u8 FLASH_u8Init(void);

/*
 * Function: FLASH_u8Toggle
 * ------------------------
 * Toggles flashlight ON/OFF state.
 */
u8 FLASH_u8Toggle(void);

/*
 * Function: FLASH_u8SetState
 * --------------------------
 * Sets flashlight state directly.
 */
u8 FLASH_u8SetState(u8 Copy_u8State);

/*
 * Function: FLASH_u8GetState
 * --------------------------
 * Returns current flashlight ON/OFF state.
 */
u8 FLASH_u8GetState(u8 *Copy_pu8State);

/*
 * Function: FLASH_u8Update
 * ------------------------
 * Periodic update function.
 * Must be called repeatedly in the main loop.
 *
 * It generates software PWM when flashlight is ON.
 */
u8 FLASH_u8Update(void);

#endif