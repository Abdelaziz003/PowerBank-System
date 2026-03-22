#include "FLASHLIGHT_interface.h"
#include "FLASHLIGHT_private.h"
#include "FLASHLIGHT_config.h"

#include "../../MCAL/DIO/DIO_interface.h"

static u8 FLASH_u8State = FLASH_OFF;
static u8 FLASH_u8PwmCounter = 0;

u8 FLASH_u8Init(void)
{
    u8 Local_u8ErrorState = FLASH_OK;

    Local_u8ErrorState = DIO_u8SetPinDirection(FLASH_OUT_PORT, FLASH_OUT_PIN, DIO_OUTPUT);
    DIO_u8SetPinValue(FLASH_OUT_PORT, FLASH_OUT_PIN, DIO_LOW);

    FLASH_u8State = FLASH_OFF;
    FLASH_u8PwmCounter = 0;

    return Local_u8ErrorState;
}

u8 FLASH_u8Toggle(void)
{
    if (FLASH_u8State == FLASH_OFF)
    {
        FLASH_u8State = FLASH_ON;
    }
    else
    {
        FLASH_u8State = FLASH_OFF;
        DIO_u8SetPinValue(FLASH_OUT_PORT, FLASH_OUT_PIN, DIO_LOW);
    }

    FLASH_u8PwmCounter = 0;

    return FLASH_OK;
}

u8 FLASH_u8SetState(u8 Copy_u8State)
{
    if ((Copy_u8State != FLASH_OFF) && (Copy_u8State != FLASH_ON))
    {
        return FLASH_NOK;
    }

    FLASH_u8State = Copy_u8State;

    if (FLASH_u8State == FLASH_OFF)
    {
        DIO_u8SetPinValue(FLASH_OUT_PORT, FLASH_OUT_PIN, DIO_LOW);
    }

    FLASH_u8PwmCounter = 0;

    return FLASH_OK;
}

u8 FLASH_u8GetState(u8 *Copy_pu8State)
{
    if (Copy_pu8State == 0)
    {
        return FLASH_NOK;
    }

    *Copy_pu8State = FLASH_u8State;
    return FLASH_OK;
}

u8 FLASH_u8Update(void)
{
    if (FLASH_u8State == FLASH_OFF)
    {
        DIO_u8SetPinValue(FLASH_OUT_PORT, FLASH_OUT_PIN, DIO_LOW);
        return FLASH_OK;
    }

    /*
     * Software PWM:
     * Output is HIGH during the first FLASH_PWM_ON_TICKS
     * of each PWM period, then LOW for the remaining ticks.
     */
    if (FLASH_u8PwmCounter < FLASH_PWM_ON_TICKS)
    {
        DIO_u8SetPinValue(FLASH_OUT_PORT, FLASH_OUT_PIN, DIO_HIGH);
    }
    else
    {
        DIO_u8SetPinValue(FLASH_OUT_PORT, FLASH_OUT_PIN, DIO_LOW);
    }

    FLASH_u8PwmCounter++;

    if (FLASH_u8PwmCounter >= FLASH_PWM_PERIOD_TICKS)
    {
        FLASH_u8PwmCounter = 0;
    }

    return FLASH_OK;
}