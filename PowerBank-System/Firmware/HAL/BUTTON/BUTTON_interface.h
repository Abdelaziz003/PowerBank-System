#ifndef BUTTON_INTERFACE_H
#define BUTTON_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

/* Button states */
#define BUTTON_RELEASED            0
#define BUTTON_PRESSED             1

/* Button active mode */
#define BUTTON_ACTIVE_LOW          0
#define BUTTON_ACTIVE_HIGH         1

/* Button events */
#define BUTTON_NO_EVENT            0
#define BUTTON_PRESS_START_EVENT   1
#define BUTTON_RELEASE_EVENT       2
#define BUTTON_LONG_PRESS_EVENT    3

/* Return status */
#define BUTTON_OK                  0
#define BUTTON_NOK                 1

/*
 * Function: BUTTON_u8Init
 * -----------------------
 * Initializes the button pin.
 */
u8 BUTTON_u8Init(void);

/*
 * Function: BUTTON_voidUpdate
 * ---------------------------
 * Periodic button service.
 * Must be called repeatedly in the main loop.
 */
void BUTTON_voidUpdate(void);

/*
 * Function: BUTTON_u8GetState
 * ---------------------------
 * Returns the current debounced button state.
 */
u8 BUTTON_u8GetState(u8 *Copy_pu8State);

/*
 * Function: BUTTON_u8GetEvent
 * ---------------------------
 * Returns the latest event once, then clears it.
 */
u8 BUTTON_u8GetEvent(u8 *Copy_pu8Event);

/*
 * Function: BUTTON_u8GetLongTriggered
 * -----------------------------------
 * Returns whether long-press condition has already been reached
 * during the current press cycle.
 */
u8 BUTTON_u8GetLongTriggered(u8 *Copy_pu8LongTriggered);

/*
 * Function: BUTTON_u16GetPressTicks
 * ---------------------------------
 * Returns how long the button has been held in update ticks.
 */
u8 BUTTON_u16GetPressTicks(u16 *Copy_pu16Ticks);

#endif