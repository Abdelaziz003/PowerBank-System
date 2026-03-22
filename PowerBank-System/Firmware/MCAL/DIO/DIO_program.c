#include "DIO_interface.h"
#include "DIO_private.h"
#include "DIO_config.h"
#include "../../LIB/BIT_MATH.h"

static u8 DIO_u8IsValidPort(u8 Copy_u8Port);
static u8 DIO_u8IsValidPin(u8 Copy_u8Pin);

u8 DIO_u8SetPinDirection(u8 Copy_u8Port, u8 Copy_u8Pin, u8 Copy_u8Direction)
{
    u8 Local_u8ErrorState = DIO_OK;

    if ((DIO_u8IsValidPort(Copy_u8Port) == 0) || (DIO_u8IsValidPin(Copy_u8Pin) == 0))
    {
        Local_u8ErrorState = DIO_NOK;
    }
    else
    {
        switch (Copy_u8Port)
        {
            case DIO_PORTA:
                if (Copy_u8Direction == DIO_INPUT)
                {
                    SET_BIT(TRISA, Copy_u8Pin);
                }
                else if (Copy_u8Direction == DIO_OUTPUT)
                {
                    /* RA3 is input only on PIC16F688 */
                    if (Copy_u8Pin == DIO_PIN3)
                    {
                        Local_u8ErrorState = DIO_NOK;
                    }
                    else
                    {
                        CLR_BIT(TRISA, Copy_u8Pin);
                    }
                }
                else
                {
                    Local_u8ErrorState = DIO_NOK;
                }
                break;

            case DIO_PORTC:
                if (Copy_u8Direction == DIO_INPUT)
                {
                    SET_BIT(TRISC, Copy_u8Pin);
                }
                else if (Copy_u8Direction == DIO_OUTPUT)
                {
                    CLR_BIT(TRISC, Copy_u8Pin);
                }
                else
                {
                    Local_u8ErrorState = DIO_NOK;
                }
                break;

            default:
                Local_u8ErrorState = DIO_NOK;
                break;
        }
    }

    return Local_u8ErrorState;
}

u8 DIO_u8SetPinValue(u8 Copy_u8Port, u8 Copy_u8Pin, u8 Copy_u8Value)
{
    u8 Local_u8ErrorState = DIO_OK;

    if ((DIO_u8IsValidPort(Copy_u8Port) == 0) || (DIO_u8IsValidPin(Copy_u8Pin) == 0))
    {
        Local_u8ErrorState = DIO_NOK;
    }
    else
    {
        switch (Copy_u8Port)
        {
            case DIO_PORTA:
                if (Copy_u8Value == DIO_HIGH)
                {
                    SET_BIT(PORTA, Copy_u8Pin);
                }
                else if (Copy_u8Value == DIO_LOW)
                {
                    CLR_BIT(PORTA, Copy_u8Pin);
                }
                else
                {
                    Local_u8ErrorState = DIO_NOK;
                }
                break;

            case DIO_PORTC:
                if (Copy_u8Value == DIO_HIGH)
                {
                    SET_BIT(PORTC, Copy_u8Pin);
                }
                else if (Copy_u8Value == DIO_LOW)
                {
                    CLR_BIT(PORTC, Copy_u8Pin);
                }
                else
                {
                    Local_u8ErrorState = DIO_NOK;
                }
                break;

            default:
                Local_u8ErrorState = DIO_NOK;
                break;
        }
    }

    return Local_u8ErrorState;
}

u8 DIO_u8TogglePin(u8 Copy_u8Port, u8 Copy_u8Pin)
{
    u8 Local_u8ErrorState = DIO_OK;

    if ((DIO_u8IsValidPort(Copy_u8Port) == 0) || (DIO_u8IsValidPin(Copy_u8Pin) == 0))
    {
        Local_u8ErrorState = DIO_NOK;
    }
    else
    {
        switch (Copy_u8Port)
        {
            case DIO_PORTA:
                /* RA3 is input only, do not toggle it as output */
                if (Copy_u8Pin == DIO_PIN3)
                {
                    Local_u8ErrorState = DIO_NOK;
                }
                else
                {
                    TOG_BIT(PORTA, Copy_u8Pin);
                }
                break;

            case DIO_PORTC:
                TOG_BIT(PORTC, Copy_u8Pin);
                break;

            default:
                Local_u8ErrorState = DIO_NOK;
                break;
        }
    }

    return Local_u8ErrorState;
}

u8 DIO_u8GetPinValue(u8 Copy_u8Port, u8 Copy_u8Pin, u8 *Copy_pu8Value)
{
    u8 Local_u8ErrorState = DIO_OK;

    if ((Copy_pu8Value == NULL) ||
        (DIO_u8IsValidPort(Copy_u8Port) == 0) ||
        (DIO_u8IsValidPin(Copy_u8Pin) == 0))
    {
        Local_u8ErrorState = DIO_NOK;
    }
    else
    {
        switch (Copy_u8Port)
        {
            case DIO_PORTA:
                *Copy_pu8Value = GET_BIT(PORTA, Copy_u8Pin);
                break;

            case DIO_PORTC:
                *Copy_pu8Value = GET_BIT(PORTC, Copy_u8Pin);
                break;

            default:
                Local_u8ErrorState = DIO_NOK;
                break;
        }
    }

    return Local_u8ErrorState;
}

/* -------------------- Static Helpers -------------------- */

static u8 DIO_u8IsValidPort(u8 Copy_u8Port)
{
    if ((Copy_u8Port == DIO_PORTA) || (Copy_u8Port == DIO_PORTC))
    {
        return 1;
    }
    return 0;
}

static u8 DIO_u8IsValidPin(u8 Copy_u8Pin)
{
    if (Copy_u8Pin <= DIO_MAX_PIN_NUMBER)
    {
        return 1;
    }
    return 0;
}