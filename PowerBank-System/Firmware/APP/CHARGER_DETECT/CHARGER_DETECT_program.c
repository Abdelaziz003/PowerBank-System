#include "CHARGER_DETECT_interface.h"
#include "CHARGER_DETECT_private.h"
#include "CHARGER_DETECT_config.h"

#include "../../MCAL/DIO/DIO_interface.h"

static u8 CHGDET_u8State = CHGDET_NOT_CONNECTED;

u8 CHGDET_u8Init(void)
{
    u8 Local_u8ErrorState = CHGDET_OK;

    Local_u8ErrorState = DIO_u8SetPinDirection(CHGDET_PORT, CHGDET_PIN, DIO_INPUT);
    CHGDET_u8State = CHGDET_NOT_CONNECTED;

    return Local_u8ErrorState;
}

u8 CHGDET_u8Update(void)
{
    u8 Local_u8PinValue = 0;

    if (DIO_u8GetPinValue(CHGDET_PORT, CHGDET_PIN, &Local_u8PinValue) != DIO_OK)
    {
        return CHGDET_NOK;
    }

#if (CHGDET_ACTIVE_MODE == CHGDET_ACTIVE_LOW)
    if (Local_u8PinValue == DIO_LOW)
    {
        CHGDET_u8State = CHGDET_CONNECTED;
    }
    else
    {
        CHGDET_u8State = CHGDET_NOT_CONNECTED;
    }
#else
    if (Local_u8PinValue == DIO_HIGH)
    {
        CHGDET_u8State = CHGDET_CONNECTED;
    }
    else
    {
        CHGDET_u8State = CHGDET_NOT_CONNECTED;
    }
#endif

    return CHGDET_OK;
}

u8 CHGDET_u8GetState(u8 *Copy_pu8State)
{
    if (Copy_pu8State == 0)
    {
        return CHGDET_NOK;
    }

    *Copy_pu8State = CHGDET_u8State;
    return CHGDET_OK;
}