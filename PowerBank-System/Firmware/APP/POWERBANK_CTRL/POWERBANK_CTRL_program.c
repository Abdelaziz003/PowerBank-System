#include "POWERBANK_CTRL_interface.h"
#include "POWERBANK_CTRL_private.h"
#include "POWERBANK_CTRL_config.h"

#include "../../HAL/BUTTON/BUTTON_interface.h"

#include "../../MCAL/DIO/DIO_interface.h"

#include "../BATTERY_MONITOR/BATTERY_MONITOR_interface.h"
#include "../BATTERY_LEVEL/BATTERY_LEVEL_interface.h"
#include "../FLASHLIGHT/FLASHLIGHT_interface.h"
#include "../CHARGER_DETECT/CHARGER_DETECT_interface.h"
#include "../CHARGE_CURRENT_DETECT/CHARGE_CURRENT_DETECT_interface.h"
#include "../CHARGE_CURRENT_DETECT/CHARGE_CURRENT_DETECT_config.h"

static u8  PBCTRL_u8Event = BUTTON_NO_EVENT;
static u8  PBCTRL_u8ChargerState = CHGDET_NOT_CONNECTED;
static u8  PBCTRL_u8PrevChargerState = CHGDET_NOT_CONNECTED;
static u8  PBCTRL_u8ChargeCurrentState = CCDET_NO_CHARGE_CURRENT;

static u8  PBCTRL_u8ShowBatteryLevel = 0;
static u8  PBCTRL_u8LongActionDoneForCurrentPress = 0;

static u16 PBCTRL_u16DisplayTicks = 0;

static u8  PBCTRL_u8BlinkState = DIO_LOW;
static u8  PBCTRL_u8BlinkCounter = 0;

static BATMON_Data_t PBCTRL_stBatData;

u8 PBCTRL_u8Init(void)
{
    u8 Local_u8ErrorState = PBCTRL_OK;

    Local_u8ErrorState |= BUTTON_u8Init();
    Local_u8ErrorState |= BATMON_u8Init();
    Local_u8ErrorState |= BLEVEL_u8Init();
    Local_u8ErrorState |= FLASH_u8Init();
    Local_u8ErrorState |= CHGDET_u8Init();
    Local_u8ErrorState |= CCDET_u8Init();

    PBCTRL_u8Event = BUTTON_NO_EVENT;
    PBCTRL_u8ChargerState = CHGDET_NOT_CONNECTED;
    PBCTRL_u8PrevChargerState = CHGDET_NOT_CONNECTED;
    PBCTRL_u8ChargeCurrentState = CCDET_NO_CHARGE_CURRENT;

    PBCTRL_u8ShowBatteryLevel = 0;
    PBCTRL_u8LongActionDoneForCurrentPress = 0;
    PBCTRL_u16DisplayTicks = 0;

    PBCTRL_u8BlinkState = DIO_LOW;
    PBCTRL_u8BlinkCounter = 0;

    return Local_u8ErrorState;
}

u8 PBCTRL_u8Update(void)
{
    /* Continuous battery monitoring */
    BATMON_u8Update();
    BATMON_u8GetData(&PBCTRL_stBatData);

    /* Charger detect update */
    CHGDET_u8Update();
    CHGDET_u8GetState(&PBCTRL_u8ChargerState);

    /* Charge current detect update */
    CCDET_u8Update();
    CCDET_u8GetState(&PBCTRL_u8ChargeCurrentState);

    /* If relay is OFF, flashlight must be forced OFF immediately */
    if (PBCTRL_stBatData.u8RelayState == BATMON_RELAY_OFF)
    {
        FLASH_u8SetState(FLASH_OFF);
    }

    /* Detect charger disconnection edge */
    if ((PBCTRL_u8PrevChargerState == CHGDET_CONNECTED) &&
        (PBCTRL_u8ChargerState == CHGDET_NOT_CONNECTED))
    {
        PBCTRL_u8ShowBatteryLevel = 0;
        PBCTRL_u16DisplayTicks = 0;
        BLEVEL_u8ClearDisplay();
    }

    /* Button service update */
    BUTTON_voidUpdate();
    BUTTON_u8GetEvent(&PBCTRL_u8Event);

    /* Blink generator */
    PBCTRL_u8BlinkCounter++;
    if (PBCTRL_u8BlinkCounter >= PBCTRL_BLINK_COUNTER_LIMIT)
    {
        PBCTRL_u8BlinkCounter = 0;
        PBCTRL_u8BlinkState = (PBCTRL_u8BlinkState == DIO_LOW) ? DIO_HIGH : DIO_LOW;
    }

    /*
     * Button behavior is used only when charger is NOT connected.
     * When charger is connected, battery level display is automatic.
     */
    if (PBCTRL_u8ChargerState == CHGDET_NOT_CONNECTED)
    {
        if (PBCTRL_u8Event == BUTTON_PRESS_START_EVENT)
        {
            PBCTRL_u8LongActionDoneForCurrentPress = 0;
        }
        else if (PBCTRL_u8Event == BUTTON_LONG_PRESS_EVENT)
        {
            PBCTRL_u8LongActionDoneForCurrentPress = 1;
            PBCTRL_u8ShowBatteryLevel = 0;
            PBCTRL_u16DisplayTicks = 0;
            BLEVEL_u8ClearDisplay();

            if (PBCTRL_stBatData.u8RelayState == BATMON_RELAY_ON)
            {
                FLASH_u8Toggle();
            }
            else
            {
                FLASH_u8SetState(FLASH_OFF);
            }
        }
        else if (PBCTRL_u8Event == BUTTON_RELEASE_EVENT)
        {
            if (PBCTRL_u8LongActionDoneForCurrentPress == 0)
            {
                PBCTRL_u8ShowBatteryLevel = 1;
                PBCTRL_u16DisplayTicks = PBCTRL_DISPLAY_TICKS_AFTER_SHORT_PRESS;
            }
            else
            {
                PBCTRL_u8ShowBatteryLevel = 0;
                PBCTRL_u16DisplayTicks = 0;
                BLEVEL_u8ClearDisplay();
            }
        }
    }
    else
    {
        /* Charger connected -> automatic display */
        PBCTRL_u8ShowBatteryLevel = 1;
        PBCTRL_u16DisplayTicks = 0;

        /* Long press while charger connected */
        if (PBCTRL_u8Event == BUTTON_LONG_PRESS_EVENT)
        {
            if (PBCTRL_stBatData.u8RelayState == BATMON_RELAY_ON)
            {
                FLASH_u8Toggle();
            }
            else
            {
                FLASH_u8SetState(FLASH_OFF);
            }
        }
    }

    /* Battery level display manager */
    if (PBCTRL_u8ShowBatteryLevel == 1)
    {
        if (PBCTRL_u8ChargerState == CHGDET_CONNECTED)
        {
#if (CCDET_ENABLE_FEATURE == 1)
            if (PBCTRL_u8ChargeCurrentState == CCDET_CHARGE_CURRENT)
            {
                /* Charger connected and charging current exists */
                BLEVEL_u8DisplayLevelCharging(PBCTRL_u8BlinkState);
            }
            else
            {
                /* Charger connected and no charging current -> battery full */
                BLEVEL_u8DisplayAllLedsOn();
            }
#else
            BLEVEL_u8DisplayLevelCharging(PBCTRL_u8BlinkState);
#endif
        }
        else
        {
            /* Charger not connected -> normal fixed display */
            BLEVEL_u8DisplayLevel();

            if (PBCTRL_u16DisplayTicks > 0)
            {
                PBCTRL_u16DisplayTicks--;

                if (PBCTRL_u16DisplayTicks == 0)
                {
                    PBCTRL_u8ShowBatteryLevel = 0;
                    BLEVEL_u8ClearDisplay();
                }
            }
        }
    }
    else
    {
        BLEVEL_u8ClearDisplay();
    }

    /* Final safety enforcement */
    if (PBCTRL_stBatData.u8RelayState == BATMON_RELAY_OFF)
    {
        FLASH_u8SetState(FLASH_OFF);
    }


    /* Save current charger state for next loop */
    PBCTRL_u8PrevChargerState = PBCTRL_u8ChargerState;

    return PBCTRL_OK;
}