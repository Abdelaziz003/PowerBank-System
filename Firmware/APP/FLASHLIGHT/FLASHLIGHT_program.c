#include "FLASHLIGHT_interface.h"
#include "FLASHLIGHT_private.h"
#include "FLASHLIGHT_config.h"

#include "../../MCAL/DIO/DIO_interface.h"

static u8 FLASH_u8State = FLASH_OFF;

u8 FLASH_u8Init(void)
{
    u8 Local_u8ErrorState = FLASH_OK;

    Local_u8ErrorState = DIO_u8SetPinDirection(FLASH_OUT_PORT, FLASH_OUT_PIN, DIO_OUTPUT);

    FLASH_u8State = FLASH_OFF;

    /* OFF = disable 555 */
    Local_u8ErrorState |= DIO_u8SetPinValue(FLASH_OUT_PORT, FLASH_OUT_PIN, DIO_HIGH);

    return Local_u8ErrorState;
}

u8 FLASH_u8Toggle(void)
{
    if (FLASH_u8State == FLASH_OFF)
    {
        FLASH_u8State = FLASH_ON;
        DIO_u8SetPinValue(FLASH_OUT_PORT, FLASH_OUT_PIN, DIO_LOW);
    }
    else
    {
        FLASH_u8State = FLASH_OFF;
        DIO_u8SetPinValue(FLASH_OUT_PORT, FLASH_OUT_PIN, DIO_HIGH);
    }

    return FLASH_OK;
}

u8 FLASH_u8SetState(u8 Copy_u8State)
{
    if ((Copy_u8State != FLASH_OFF) && (Copy_u8State != FLASH_ON))
    {
        return FLASH_NOK;
    }

    FLASH_u8State = Copy_u8State;

    if (FLASH_u8State == FLASH_ON)
    {
        /* ON = enable 555 */
        DIO_u8SetPinValue(FLASH_OUT_PORT, FLASH_OUT_PIN, DIO_LOW);
    }
    else
    {
        /* OFF = disable 555 */
        DIO_u8SetPinValue(FLASH_OUT_PORT, FLASH_OUT_PIN, DIO_HIGH);
    }

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
    /*
     * No software PWM is needed anymore.
     * 555 generates the flashing/PWM externally.
     */
    return FLASH_OK;
}