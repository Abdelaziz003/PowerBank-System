#ifndef DIO_INTERFACE_H
#define DIO_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

/* Port IDs */
#define DIO_PORTA      0
#define DIO_PORTC      1

/* Pin IDs */
#define DIO_PIN0       0
#define DIO_PIN1       1
#define DIO_PIN2       2
#define DIO_PIN3       3
#define DIO_PIN4       4
#define DIO_PIN5       5

/* Pin direction */
#define DIO_INPUT      1
#define DIO_OUTPUT     0

/* Pin value */
#define DIO_LOW        0
#define DIO_HIGH       1

/* Return status */
#define DIO_OK         0
#define DIO_NOK        1

/*
 * Function: DIO_u8SetPinDirection
 * -------------------------------
 * Sets the direction of a single pin.
 *
 * Notes:
 * - TRIS bit = 1 => input
 * - TRIS bit = 0 => output
 * - On PIC16F688, RA3 is input only.
 */
u8 DIO_u8SetPinDirection(u8 Copy_u8Port, u8 Copy_u8Pin, u8 Copy_u8Direction);

/*
 * Function: DIO_u8SetPinValue
 * ---------------------------
 * Writes logic HIGH or LOW to a digital output pin.
 */
u8 DIO_u8SetPinValue(u8 Copy_u8Port, u8 Copy_u8Pin, u8 Copy_u8Value);

/*
 * Function: DIO_u8TogglePin
 * -------------------------
 * Toggles the output state of a digital pin.
 */
u8 DIO_u8TogglePin(u8 Copy_u8Port, u8 Copy_u8Pin);

/*
 * Function: DIO_u8GetPinValue
 * ---------------------------
 * Reads the current logic level on a pin.
 */
u8 DIO_u8GetPinValue(u8 Copy_u8Port, u8 Copy_u8Pin, u8 *Copy_pu8Value);

#endif