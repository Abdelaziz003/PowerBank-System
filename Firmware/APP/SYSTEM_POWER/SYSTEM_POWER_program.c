#include "SYSTEM_POWER_interface.h"
#include "SYSTEM_POWER_private.h"
#include "SYSTEM_POWER_config.h"

#include "../CHARGER_DETECT/CHARGER_DETECT_interface.h"

static SPWR_Data_t SPWR_stData;
static u16 SPWR_u16StartupCounter = 0;

u8 SPWR_u8Init(void)
{
    SPWR_stData.u8SystemState   = SPWR_ACTIVE;
    SPWR_stData.u8SleepRequest  = 0;
    SPWR_stData.u8WakeRequest   = 0;
    SPWR_stData.u8StartupActive = 0;

    SPWR_u16StartupCounter = 0;

    return SPWR_OK;
}

u8 SPWR_u8Update(u8 Copy_u8SleepCommand, u8 Copy_u8WakeCommand, u8 Copy_u8ChargerState)
{
    /* Clear one-shot transition flags every cycle */
    SPWR_stData.u8SleepRequest = 0;
    SPWR_stData.u8WakeRequest  = 0;

    switch (SPWR_stData.u8SystemState)
    {
        case SPWR_ACTIVE:
        {
            if (Copy_u8SleepCommand == 1U)
            {
                SPWR_stData.u8SleepRequest  = 1;
                SPWR_stData.u8SystemState   = SPWR_SLEEP;
                SPWR_stData.u8StartupActive = 0;
            }
            break;
        }

        case SPWR_SLEEP:
        {
            /*
             * Wake if:
             * - external wake command received
             * - charger connected
             */
            if ((Copy_u8WakeCommand == 1U) ||
                (Copy_u8ChargerState == CHGDET_CONNECTED))
            {
                SPWR_stData.u8WakeRequest   = 1;
                SPWR_stData.u8SystemState   = SPWR_STARTUP;
                SPWR_stData.u8StartupActive = 1;
                SPWR_u16StartupCounter      = 0;
            }
            break;
        }

        case SPWR_STARTUP:
        {
            SPWR_u16StartupCounter++;

            if (SPWR_u16StartupCounter >= SPWR_STARTUP_DURATION_TICKS)
            {
                SPWR_stData.u8SystemState   = SPWR_ACTIVE;
                SPWR_stData.u8StartupActive = 0;
                SPWR_u16StartupCounter      = 0;
            }
            break;
        }

        default:
        {
            SPWR_stData.u8SystemState   = SPWR_ACTIVE;
            SPWR_stData.u8StartupActive = 0;
            SPWR_u16StartupCounter      = 0;
            break;
        }
    }

    return SPWR_OK;
}

u8 SPWR_u8GetData(SPWR_Data_t *Copy_pstData)
{
    if (Copy_pstData == 0)
    {
        return SPWR_NOK;
    }

    *Copy_pstData = SPWR_stData;
    return SPWR_OK;
}