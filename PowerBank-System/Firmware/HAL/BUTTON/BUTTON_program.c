#include "BUTTON_interface.h"
#include "BUTTON_private.h"
#include "BUTTON_config.h"
#include "../../MCAL/DIO/DIO_interface.h"

static u8  BUTTON_u8CurrentState = BUTTON_RELEASED;
static u8  BUTTON_u8LastRawState = BUTTON_RELEASED;
static u8  BUTTON_u8DebounceCounter = 0;

static u16 BUTTON_u16PressTicks = 0;
static u8  BUTTON_u8LongTriggered = 0;
static u8  BUTTON_u8Event = BUTTON_NO_EVENT;

/*
 * Helper:
 * Convert raw GPIO level into logical button state
 * according to active mode.
 */
static u8 BUTTON_u8ReadRawLogicalState(void)
{
    u8 Local_u8PinValue = 0;

    DIO_u8GetPinValue(BUTTON_PORT, BUTTON_PIN, &Local_u8PinValue);

#if (BUTTON_ACTIVE_MODE == BUTTON_ACTIVE_LOW)
    return (Local_u8PinValue == DIO_LOW) ? BUTTON_PRESSED : BUTTON_RELEASED;
#else
    return (Local_u8PinValue == DIO_HIGH) ? BUTTON_PRESSED : BUTTON_RELEASED;
#endif
}

u8 BUTTON_u8Init(void)
{
    u8 Local_u8ErrorState = BUTTON_OK;

    Local_u8ErrorState = DIO_u8SetPinDirection(BUTTON_PORT, BUTTON_PIN, DIO_INPUT);

    BUTTON_u8CurrentState = BUTTON_RELEASED;
    BUTTON_u8LastRawState = BUTTON_RELEASED;
    BUTTON_u8DebounceCounter = 0;
    BUTTON_u16PressTicks = 0;
    BUTTON_u8LongTriggered = 0;
    BUTTON_u8Event = BUTTON_NO_EVENT;

    return Local_u8ErrorState;
}

void BUTTON_voidUpdate(void)
{
    u8 Local_u8RawState = BUTTON_u8ReadRawLogicalState();

    /* Debounce */
    if (Local_u8RawState == BUTTON_u8LastRawState)
    {
        if (BUTTON_u8DebounceCounter < BUTTON_DEBOUNCE_COUNT)
        {
            BUTTON_u8DebounceCounter++;
        }
    }
    else
    {
        BUTTON_u8DebounceCounter = 0;
    }

    BUTTON_u8LastRawState = Local_u8RawState;

    if (BUTTON_u8DebounceCounter >= BUTTON_DEBOUNCE_COUNT)
    {
        /* Stable state confirmed */
        if (BUTTON_u8CurrentState != Local_u8RawState)
        {
            BUTTON_u8CurrentState = Local_u8RawState;

            if (BUTTON_u8CurrentState == BUTTON_PRESSED)
            {
                /* Valid press started */
                BUTTON_u16PressTicks = 0;
                BUTTON_u8LongTriggered = 0;
                BUTTON_u8Event = BUTTON_PRESS_START_EVENT;
            }
            else
            {
                /* Valid release */
                BUTTON_u8Event = BUTTON_RELEASE_EVENT;
                BUTTON_u16PressTicks = 0;
                BUTTON_u8LongTriggered = 0;
            }
        }
        else
        {
            /* Stable unchanged state */
            if (BUTTON_u8CurrentState == BUTTON_PRESSED)
            {
                if (BUTTON_u16PressTicks < 65535U)
                {
                    BUTTON_u16PressTicks++;
                }

                if ((BUTTON_u16PressTicks >= BUTTON_LONG_PRESS_TICKS) &&
                    (BUTTON_u8LongTriggered == 0))
                {
                    BUTTON_u8LongTriggered = 1;
                    BUTTON_u8Event = BUTTON_LONG_PRESS_EVENT;
                }
            }
        }
    }
}

u8 BUTTON_u8GetState(u8 *Copy_pu8State)
{
    if (Copy_pu8State == 0)
    {
        return BUTTON_NOK;
    }

    *Copy_pu8State = BUTTON_u8CurrentState;
    return BUTTON_OK;
}

u8 BUTTON_u8GetEvent(u8 *Copy_pu8Event)
{
    if (Copy_pu8Event == 0)
    {
        return BUTTON_NOK;
    }

    *Copy_pu8Event = BUTTON_u8Event;
    BUTTON_u8Event = BUTTON_NO_EVENT;

    return BUTTON_OK;
}

u8 BUTTON_u8GetLongTriggered(u8 *Copy_pu8LongTriggered)
{
    if (Copy_pu8LongTriggered == 0)
    {
        return BUTTON_NOK;
    }

    *Copy_pu8LongTriggered = BUTTON_u8LongTriggered;
    return BUTTON_OK;
}

u8 BUTTON_u16GetPressTicks(u16 *Copy_pu16Ticks)
{
    if (Copy_pu16Ticks == 0)
    {
        return BUTTON_NOK;
    }

    *Copy_pu16Ticks = BUTTON_u16PressTicks;
    return BUTTON_OK;
}