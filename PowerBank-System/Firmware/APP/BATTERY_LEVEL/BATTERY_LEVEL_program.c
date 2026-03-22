#include "BATTERY_LEVEL_interface.h"
#include "BATTERY_LEVEL_private.h"
#include "BATTERY_LEVEL_config.h"

#include "../../MCAL/DIO/DIO_interface.h"
#include "../BATTERY_MONITOR/BATTERY_MONITOR_interface.h"

static void BLEVEL_voidSetLed25(u8 Copy_u8Value);
static void BLEVEL_voidSetLed50(u8 Copy_u8Value);
static void BLEVEL_voidSetLed75(u8 Copy_u8Value);
static void BLEVEL_voidSetLed100(u8 Copy_u8Value);

u8 BLEVEL_u8Init(void)
{
    u8 Local_u8ErrorState = BLEVEL_OK;

    Local_u8ErrorState |= DIO_u8SetPinDirection(BLEVEL_LED25_PORT, BLEVEL_LED25_PIN, DIO_OUTPUT);
    Local_u8ErrorState |= DIO_u8SetPinDirection(BLEVEL_LED50_PORT, BLEVEL_LED50_PIN, DIO_OUTPUT);
    Local_u8ErrorState |= DIO_u8SetPinDirection(BLEVEL_LED75_PORT, BLEVEL_LED75_PIN, DIO_OUTPUT);
    Local_u8ErrorState |= DIO_u8SetPinDirection(BLEVEL_LED100_PORT, BLEVEL_LED100_PIN, DIO_OUTPUT);

    BLEVEL_u8ClearDisplay();

    return Local_u8ErrorState;
}

u8 BLEVEL_u8GetLevel(u8 *Copy_pu8Level)
{
    BATMON_Data_t Local_stBatData;

    if (Copy_pu8Level == 0)
    {
        return BLEVEL_NOK;
    }

    if (BATMON_u8GetData(&Local_stBatData) != BATMON_OK)
    {
        return BLEVEL_NOK;
    }

    if (Local_stBatData.u16Tap3_mV >= BLEVEL_PACK_100_MV)
    {
        *Copy_pu8Level = BLEVEL_100_PERCENT;
    }
    else if (Local_stBatData.u16Tap3_mV >= BLEVEL_PACK_75_MV)
    {
        *Copy_pu8Level = BLEVEL_75_PERCENT;
    }
    else if (Local_stBatData.u16Tap3_mV >= BLEVEL_PACK_50_MV)
    {
        *Copy_pu8Level = BLEVEL_50_PERCENT;
    }
    else if (Local_stBatData.u16Tap3_mV >= BLEVEL_PACK_25_MV)
    {
        *Copy_pu8Level = BLEVEL_25_PERCENT;
    }
    else
    {
        *Copy_pu8Level = BLEVEL_0_PERCENT;
    }

    return BLEVEL_OK;
}

u8 BLEVEL_u8DisplayLevel(void)
{
    u8 Local_u8Level = 0;

    if (BLEVEL_u8GetLevel(&Local_u8Level) != BLEVEL_OK)
    {
        return BLEVEL_NOK;
    }

    BLEVEL_u8ClearDisplay();

    if (Local_u8Level >= BLEVEL_25_PERCENT)
    {
        BLEVEL_voidSetLed25(DIO_HIGH);
    }

    if (Local_u8Level >= BLEVEL_50_PERCENT)
    {
        BLEVEL_voidSetLed50(DIO_HIGH);
    }

    if (Local_u8Level >= BLEVEL_75_PERCENT)
    {
        BLEVEL_voidSetLed75(DIO_HIGH);
    }

    if (Local_u8Level >= BLEVEL_100_PERCENT)
    {
        BLEVEL_voidSetLed100(DIO_HIGH);
    }

    return BLEVEL_OK;
}

u8 BLEVEL_u8DisplayLevelCharging(u8 Copy_u8BlinkState)
{
    u8 Local_u8Level = 0;

    if (BLEVEL_u8GetLevel(&Local_u8Level) != BLEVEL_OK)
    {
        return BLEVEL_NOK;
    }

    BLEVEL_u8ClearDisplay();

    switch (Local_u8Level)
    {
        case BLEVEL_25_PERCENT:
            /* Last active LED = LED25 -> blinking */
            BLEVEL_voidSetLed25(Copy_u8BlinkState);
            break;

        case BLEVEL_50_PERCENT:
            /* LED25 fixed, LED50 blinking */
            BLEVEL_voidSetLed25(DIO_HIGH);
            BLEVEL_voidSetLed50(Copy_u8BlinkState);
            break;

        case BLEVEL_75_PERCENT:
            /* LED25 + LED50 fixed, LED75 blinking */
            BLEVEL_voidSetLed25(DIO_HIGH);
            BLEVEL_voidSetLed50(DIO_HIGH);
            BLEVEL_voidSetLed75(Copy_u8BlinkState);
            break;

        case BLEVEL_100_PERCENT:
            /* First three fixed, LED100 blinking */
            BLEVEL_voidSetLed25(DIO_HIGH);
            BLEVEL_voidSetLed50(DIO_HIGH);
            BLEVEL_voidSetLed75(DIO_HIGH);
            BLEVEL_voidSetLed100(Copy_u8BlinkState);
            break;

        default:
            /* 0% -> nothing */
            break;
    }

    return BLEVEL_OK;
}

u8 BLEVEL_u8ClearDisplay(void)
{
    BLEVEL_voidSetLed25(DIO_LOW);
    BLEVEL_voidSetLed50(DIO_LOW);
    BLEVEL_voidSetLed75(DIO_LOW);
    BLEVEL_voidSetLed100(DIO_LOW);

    return BLEVEL_OK;
}

static void BLEVEL_voidSetLed25(u8 Copy_u8Value)
{
    DIO_u8SetPinValue(BLEVEL_LED25_PORT, BLEVEL_LED25_PIN, Copy_u8Value);
}

static void BLEVEL_voidSetLed50(u8 Copy_u8Value)
{
    DIO_u8SetPinValue(BLEVEL_LED50_PORT, BLEVEL_LED50_PIN, Copy_u8Value);
}

static void BLEVEL_voidSetLed75(u8 Copy_u8Value)
{
    DIO_u8SetPinValue(BLEVEL_LED75_PORT, BLEVEL_LED75_PIN, Copy_u8Value);
}

static void BLEVEL_voidSetLed100(u8 Copy_u8Value)
{
    DIO_u8SetPinValue(BLEVEL_LED100_PORT, BLEVEL_LED100_PIN, Copy_u8Value);
}

u8 BLEVEL_u8DisplayAllLedsOn(void)
{
    DIO_u8SetPinValue(BLEVEL_LED25_PORT,  BLEVEL_LED25_PIN,  DIO_HIGH);
    DIO_u8SetPinValue(BLEVEL_LED50_PORT,  BLEVEL_LED50_PIN,  DIO_HIGH);
    DIO_u8SetPinValue(BLEVEL_LED75_PORT,  BLEVEL_LED75_PIN,  DIO_HIGH);
    DIO_u8SetPinValue(BLEVEL_LED100_PORT, BLEVEL_LED100_PIN, DIO_HIGH);

    return BLEVEL_OK;
}