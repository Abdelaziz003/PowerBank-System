#include "POWERBANK_CTRL_interface.h"
#include "POWERBANK_CTRL_private.h"
#include "POWERBANK_CTRL_config.h"

#include "../../HAL/BUTTON/BUTTON_interface.h"
#include "../../MCAL/DIO/DIO_interface.h"

#include "../BATTERY_MONITOR/BATTERY_MONITOR_interface.h"
#include "../BATTERY_MONITOR/BATTERY_MONITOR_config.h"

#include "../BATTERY_LEVEL/BATTERY_LEVEL_interface.h"
#include "../FLASHLIGHT/FLASHLIGHT_interface.h"
#include "../CHARGER_DETECT/CHARGER_DETECT_interface.h"
#include "../CHARGE_CURRENT_DETECT/CHARGE_CURRENT_DETECT_interface.h"
#include "../CHARGE_CURRENT_DETECT/CHARGE_CURRENT_DETECT_config.h"
#include "../CHARGE_CTRL/CHARGE_CTRL_interface.h"
#include "../SYSTEM_POWER/SYSTEM_POWER_interface.h"
#include "../SYSTEM_POWER/SYSTEM_POWER_config.h"

static u8  PBCTRL_u8Event = BUTTON_NO_EVENT;
static u8  PBCTRL_u8ButtonState = BUTTON_RELEASED;
static u16 PBCTRL_u16PressTicks = 0;

static u8  PBCTRL_u8ChargerState = CHGDET_NOT_CONNECTED;
static u8  PBCTRL_u8PrevChargerState = CHGDET_NOT_CONNECTED;

static u8  PBCTRL_u8ShowBatteryLevel = 0;
static u8  PBCTRL_u8LongActionDoneForCurrentPress = 0;

static u8  PBCTRL_u8PendingSingleClick = 0;
static u16 PBCTRL_u16SingleClickTimer = 0;

static u16 PBCTRL_u16DisplayTicks = 0;

static u8  PBCTRL_u8BlinkState = DIO_LOW;
static u8  PBCTRL_u8BlinkCounter = 0;

static BATMON_Data_t PBCTRL_stBatData;
static CHCTRL_Data_t PBCTRL_stChargeData;
static SPWR_Data_t   PBCTRL_stPowerData;

u8 PBCTRL_u8Init(void)
{
    u8 Local_u8ErrorState = PBCTRL_OK;

    Local_u8ErrorState |= BUTTON_u8Init();
    Local_u8ErrorState |= BATMON_u8Init();
    Local_u8ErrorState |= BLEVEL_u8Init();
    Local_u8ErrorState |= FLASH_u8Init();
    Local_u8ErrorState |= CHGDET_u8Init();
    Local_u8ErrorState |= CCDET_u8Init();      /* optional, can remain */
    Local_u8ErrorState |= CHCTRL_u8Init();
    Local_u8ErrorState |= SPWR_u8Init();

    PBCTRL_u8Event = BUTTON_NO_EVENT;
    PBCTRL_u8ButtonState = BUTTON_RELEASED;
    PBCTRL_u16PressTicks = 0;

    PBCTRL_u8ChargerState = CHGDET_NOT_CONNECTED;
    PBCTRL_u8PrevChargerState = CHGDET_NOT_CONNECTED;

    PBCTRL_u8ShowBatteryLevel = 0;
    PBCTRL_u8LongActionDoneForCurrentPress = 0;

    PBCTRL_u8PendingSingleClick = 0;
    PBCTRL_u16SingleClickTimer = 0;

    PBCTRL_u16DisplayTicks = 0;

    PBCTRL_u8BlinkState = DIO_LOW;
    PBCTRL_u8BlinkCounter = 0;

    PBCTRL_stPowerData.u8SystemState   = SPWR_ACTIVE;
    PBCTRL_stPowerData.u8SleepRequest  = 0;
    PBCTRL_stPowerData.u8WakeRequest   = 0;
    PBCTRL_stPowerData.u8StartupActive = 0;

    return Local_u8ErrorState;
}

u8 PBCTRL_u8Update(void)
{
    u8 Local_u8SleepCommand = 0;
    u8 Local_u8WakeCommand  = 0;

    /* =========================================
       Always-active inputs and measurements
       ========================================= */

    BATMON_u8Update();
    BATMON_u8GetData(&PBCTRL_stBatData);

    CHGDET_u8Update();
    CHGDET_u8GetState(&PBCTRL_u8ChargerState);

    /* Charge control always active, even in sleep */
    CHCTRL_u8Update();
    CHCTRL_u8GetData(&PBCTRL_stChargeData);

    BUTTON_voidUpdate();
    BUTTON_u8GetEvent(&PBCTRL_u8Event);
    BUTTON_u8GetState(&PBCTRL_u8ButtonState);
    BUTTON_u16GetPressTicks(&PBCTRL_u16PressTicks);

    /* Blink generator */
    PBCTRL_u8BlinkCounter++;
    if (PBCTRL_u8BlinkCounter >= PBCTRL_BLINK_COUNTER_LIMIT)
    {
        PBCTRL_u8BlinkCounter = 0;
        PBCTRL_u8BlinkState = (PBCTRL_u8BlinkState == DIO_LOW) ? DIO_HIGH : DIO_LOW;
    }

    /* =========================================
       Button logic before power-state update
       ========================================= */

    switch (PBCTRL_stPowerData.u8SystemState)
    {
        case SPWR_ACTIVE:
        {
            if (PBCTRL_u8Event == BUTTON_PRESS_START_EVENT)
            {
                PBCTRL_u8LongActionDoneForCurrentPress = 0;
            }
            else if (PBCTRL_u8Event == BUTTON_LONG_PRESS_EVENT)
            {
                PBCTRL_u8LongActionDoneForCurrentPress = 1;

                PBCTRL_u8PendingSingleClick = 0;
                PBCTRL_u16SingleClickTimer = 0;

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
                    if (PBCTRL_u8PendingSingleClick == 0)
                    {
                        PBCTRL_u8PendingSingleClick = 1;
                        PBCTRL_u16SingleClickTimer = PBCTRL_DOUBLE_CLICK_WINDOW_TICKS;
                    }
                    else
                    {
                        PBCTRL_u8PendingSingleClick = 0;
                        PBCTRL_u16SingleClickTimer = 0;
                        Local_u8SleepCommand = 1;

                        PBCTRL_u8ShowBatteryLevel = 0;
                        PBCTRL_u16DisplayTicks = 0;
                        BLEVEL_u8ClearDisplay();
                    }
                }

                PBCTRL_u8LongActionDoneForCurrentPress = 0;
            }

            break;
        }

        case SPWR_SLEEP:
        {
            if (PBCTRL_u8Event == BUTTON_PRESS_START_EVENT)
            {
                PBCTRL_u8LongActionDoneForCurrentPress = 0;
            }
            else if (PBCTRL_u8Event == BUTTON_LONG_PRESS_EVENT)
            {
                PBCTRL_u8LongActionDoneForCurrentPress = 1;
            }
            else if (PBCTRL_u8Event == BUTTON_RELEASE_EVENT)
            {
                if (PBCTRL_u8LongActionDoneForCurrentPress == 0)
                {
                    if (PBCTRL_u8PendingSingleClick == 0)
                    {
                        PBCTRL_u8PendingSingleClick = 1;
                        PBCTRL_u16SingleClickTimer = PBCTRL_DOUBLE_CLICK_WINDOW_TICKS;
                    }
                    else
                    {
                        PBCTRL_u8PendingSingleClick = 0;
                        PBCTRL_u16SingleClickTimer = 0;
                        Local_u8WakeCommand = 1;
                    }
                }

                PBCTRL_u8LongActionDoneForCurrentPress = 0;
            }

            break;
        }

        case SPWR_STARTUP:
        default:
        {
            PBCTRL_u8LongActionDoneForCurrentPress = 0;
            PBCTRL_u8PendingSingleClick = 0;
            PBCTRL_u16SingleClickTimer = 0;
            break;
        }
    }

    /* =========================================
       Resolve pending single-click timeout
       ========================================= */

    if (PBCTRL_u8PendingSingleClick == 1)
    {
        if (PBCTRL_u16SingleClickTimer > 0)
        {
            PBCTRL_u16SingleClickTimer--;
        }

        if (PBCTRL_u16SingleClickTimer == 0)
        {
            PBCTRL_u8PendingSingleClick = 0;
            PBCTRL_u8ShowBatteryLevel = 1;
            PBCTRL_u16DisplayTicks = PBCTRL_DISPLAY_TICKS_AFTER_SHORT_PRESS;
        }
    }

    /* =========================================
       Update system power state
       ========================================= */

    SPWR_u8Update(Local_u8SleepCommand, Local_u8WakeCommand, PBCTRL_u8ChargerState);
    SPWR_u8GetData(&PBCTRL_stPowerData);

    /* =========================================
       Handle one-shot sleep / wake requests
       ========================================= */

    if (PBCTRL_stPowerData.u8SleepRequest == 1)
    {
        BATMON_u8SetRelayControlEnable(BATMON_RELAY_CTRL_DISABLE);

        FLASH_u8SetState(FLASH_OFF);
        DIO_u8SetPinValue(BATMON_RELAY_PORT, BATMON_RELAY_PIN, DIO_LOW);
        PBCTRL_stBatData.u8RelayState = BATMON_RELAY_OFF;

        PBCTRL_u8ShowBatteryLevel = 0;
        PBCTRL_u16DisplayTicks = 0;

        PBCTRL_u8LongActionDoneForCurrentPress = 0;
        PBCTRL_u8PendingSingleClick = 0;
        PBCTRL_u16SingleClickTimer = 0;

        BLEVEL_u8ClearDisplay();
    }

    if (PBCTRL_stPowerData.u8WakeRequest == 1)
    {
        BATMON_u8SetRelayControlEnable(BATMON_RELAY_CTRL_DISABLE);

        FLASH_u8SetState(FLASH_OFF);
        DIO_u8SetPinValue(BATMON_RELAY_PORT, BATMON_RELAY_PIN, DIO_LOW);
        PBCTRL_stBatData.u8RelayState = BATMON_RELAY_OFF;

        PBCTRL_u8ShowBatteryLevel = 0;
        PBCTRL_u16DisplayTicks = 0;

        PBCTRL_u8LongActionDoneForCurrentPress = 0;
        PBCTRL_u8PendingSingleClick = 0;
        PBCTRL_u16SingleClickTimer = 0;
    }

    /* =========================================
       State machine
       ========================================= */

    switch (PBCTRL_stPowerData.u8SystemState)
    {
        case SPWR_SLEEP:
        {
            BATMON_u8SetRelayControlEnable(BATMON_RELAY_CTRL_DISABLE);

            FLASH_u8SetState(FLASH_OFF);
            DIO_u8SetPinValue(BATMON_RELAY_PORT, BATMON_RELAY_PIN, DIO_LOW);
            PBCTRL_stBatData.u8RelayState = BATMON_RELAY_OFF;

            /*
             * During sleep, if charger is connected:
             * show charging state / full state as well
             */
            if (PBCTRL_u8ChargerState == CHGDET_CONNECTED)
            {
                if (PBCTRL_stChargeData.u8ChargeState == CHCTRL_CHARGING_ACTIVE)
                {
                    BLEVEL_u8DisplayLevelCharging(PBCTRL_u8BlinkState);
                }
                else if (PBCTRL_stChargeData.u8ChargeState == CHCTRL_CHARGE_CUTOFF_LATCHED)
                {
                    BLEVEL_u8DisplayAllLedsOn();
                }
                else
                {
                    BLEVEL_u8ClearDisplay();
                }
            }
            else
            {
                if (PBCTRL_u8ShowBatteryLevel == 1)
                {
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
                else
                {
                    BLEVEL_u8ClearDisplay();
                }
            }

            PBCTRL_u8PrevChargerState = PBCTRL_u8ChargerState;
            return PBCTRL_OK;
        }

        case SPWR_STARTUP:
        {
            BATMON_u8SetRelayControlEnable(BATMON_RELAY_CTRL_DISABLE);

            FLASH_u8SetState(FLASH_OFF);
            DIO_u8SetPinValue(BATMON_RELAY_PORT, BATMON_RELAY_PIN, DIO_LOW);
            PBCTRL_stBatData.u8RelayState = BATMON_RELAY_OFF;

            BLEVEL_u8DisplayAllLedsOn();

            PBCTRL_u8PrevChargerState = PBCTRL_u8ChargerState;
            return PBCTRL_OK;
        }

        case SPWR_ACTIVE:
        default:
        {
            BATMON_u8SetRelayControlEnable(BATMON_RELAY_CTRL_ENABLE);
            break;
        }
    }

    /* =========================================
       ACTIVE MODE ONLY
       ========================================= */

    /* optional legacy current detect may stay unused */
    CCDET_u8Update();

    if (PBCTRL_stBatData.u8RelayState == BATMON_RELAY_OFF)
    {
        FLASH_u8SetState(FLASH_OFF);
    }

    if ((PBCTRL_u8PrevChargerState == CHGDET_CONNECTED) &&
        (PBCTRL_u8ChargerState == CHGDET_NOT_CONNECTED))
    {
        PBCTRL_u8ShowBatteryLevel = 0;
        PBCTRL_u16DisplayTicks = 0;
        BLEVEL_u8ClearDisplay();
    }

    /* Charger connected -> automatic display driven by charge control */
    if (PBCTRL_u8ChargerState == CHGDET_CONNECTED)
    {
        PBCTRL_u8ShowBatteryLevel = 1;
        PBCTRL_u16DisplayTicks = 0;
    }

    if (PBCTRL_u8ShowBatteryLevel == 1)
    {
        if (PBCTRL_u8ChargerState == CHGDET_CONNECTED)
        {
            if (PBCTRL_stChargeData.u8ChargeState == CHCTRL_CHARGING_ACTIVE)
            {
                BLEVEL_u8DisplayLevelCharging(PBCTRL_u8BlinkState);
            }
            else if (PBCTRL_stChargeData.u8ChargeState == CHCTRL_CHARGE_CUTOFF_LATCHED)
            {
                BLEVEL_u8DisplayAllLedsOn();
            }
            else
            {
                BLEVEL_u8ClearDisplay();
            }
        }
        else
        {
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

    if (PBCTRL_stBatData.u8RelayState == BATMON_RELAY_OFF)
    {
        FLASH_u8SetState(FLASH_OFF);
    }

    PBCTRL_u8PrevChargerState = PBCTRL_u8ChargerState;

    return PBCTRL_OK;
}