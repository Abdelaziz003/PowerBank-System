#include "CHARGE_CTRL_interface.h"
#include "CHARGE_CTRL_private.h"
#include "CHARGE_CTRL_config.h"

#include "../../MCAL/DIO/DIO_interface.h"

#include "../CHARGER_DETECT/CHARGER_DETECT_interface.h"
#include "../BATTERY_MONITOR/BATTERY_MONITOR_interface.h"

static CHCTRL_Data_t CHCTRL_stData;
static u8 CHCTRL_u8PrevChargerState = CHGDET_NOT_CONNECTED;

static u8 CHCTRL_u8WriteRelay(u8 Copy_u8State);

u8 CHCTRL_u8Init(void)
{
    u8 Local_u8ErrorState = CHCTRL_OK;

    Local_u8ErrorState |= DIO_u8SetPinDirection(CHCTRL_RELAY_PORT, CHCTRL_RELAY_PIN, DIO_OUTPUT);

    CHCTRL_stData.u8ChargeState = CHCTRL_NO_CHARGER;
    CHCTRL_stData.u8ChargeRelayState = CHCTRL_RELAY_OFF;
    CHCTRL_stData.u8CutoffLatched = 0;

    CHCTRL_u8PrevChargerState = CHGDET_NOT_CONNECTED;

    Local_u8ErrorState |= CHCTRL_u8WriteRelay(CHCTRL_RELAY_OFF);

    return Local_u8ErrorState;
}

u8 CHCTRL_u8Update(void)
{
    u8 Local_u8ChargerState = CHGDET_NOT_CONNECTED;
    BATMON_Data_t Local_stBatData;

    CHGDET_u8GetState(&Local_u8ChargerState);
    BATMON_u8GetData(&Local_stBatData);

    /*
     * Detect new charger connection session:
     * reset cutoff latch only on charger plug-in edge.
     */
    if ((CHCTRL_u8PrevChargerState == CHGDET_NOT_CONNECTED) &&
        (Local_u8ChargerState == CHGDET_CONNECTED))
    {
        CHCTRL_stData.u8CutoffLatched = 0;
    }

    /*
     * Charger disconnected:
     * - relay OFF
     * - clear latch
     * - state = no charger
     */
    if (Local_u8ChargerState == CHGDET_NOT_CONNECTED)
    {
        CHCTRL_stData.u8ChargeState = CHCTRL_NO_CHARGER;
        CHCTRL_stData.u8ChargeRelayState = CHCTRL_RELAY_OFF;
        CHCTRL_stData.u8CutoffLatched = 0;

        CHCTRL_u8WriteRelay(CHCTRL_RELAY_OFF);

        CHCTRL_u8PrevChargerState = Local_u8ChargerState;
        return CHCTRL_OK;
    }

    /*
     * Charger connected:
     * if latch already set, keep relay OFF no matter what.
     */
    if (CHCTRL_stData.u8CutoffLatched == 1)
    {
        CHCTRL_stData.u8ChargeState = CHCTRL_CHARGE_CUTOFF_LATCHED;
        CHCTRL_stData.u8ChargeRelayState = CHCTRL_RELAY_OFF;

        CHCTRL_u8WriteRelay(CHCTRL_RELAY_OFF);

        CHCTRL_u8PrevChargerState = Local_u8ChargerState;
        return CHCTRL_OK;
    }

    /*
     * If any cell reaches cutoff, latch and stop charging.
     */
    if ((Local_stBatData.u16Cell1_mV >= CHCTRL_CELL_CUTOFF_MV) ||
        (Local_stBatData.u16Cell2_mV >= CHCTRL_CELL_CUTOFF_MV) ||
        (Local_stBatData.u16Cell3_mV >= CHCTRL_CELL_CUTOFF_MV))
    {
        CHCTRL_stData.u8CutoffLatched = 1;
        CHCTRL_stData.u8ChargeState = CHCTRL_CHARGE_CUTOFF_LATCHED;
        CHCTRL_stData.u8ChargeRelayState = CHCTRL_RELAY_OFF;

        CHCTRL_u8WriteRelay(CHCTRL_RELAY_OFF);

        CHCTRL_u8PrevChargerState = Local_u8ChargerState;
        return CHCTRL_OK;
    }

    /*
     * Charger connected and not latched:
     * charging is allowed.
     */
    CHCTRL_stData.u8ChargeState = CHCTRL_CHARGING_ACTIVE;
    CHCTRL_stData.u8ChargeRelayState = CHCTRL_RELAY_ON;

    CHCTRL_u8WriteRelay(CHCTRL_RELAY_ON);

    CHCTRL_u8PrevChargerState = Local_u8ChargerState;
    return CHCTRL_OK;
}

u8 CHCTRL_u8GetData(CHCTRL_Data_t *Copy_pstData)
{
    if (Copy_pstData == 0)
    {
        return CHCTRL_NOK;
    }

    *Copy_pstData = CHCTRL_stData;
    return CHCTRL_OK;
}

static u8 CHCTRL_u8WriteRelay(u8 Copy_u8State)
{
#if (CHCTRL_RELAY_POLARITY == CHCTRL_RELAY_ACTIVE_HIGH)
    if (Copy_u8State == CHCTRL_RELAY_ON)
    {
        return DIO_u8SetPinValue(CHCTRL_RELAY_PORT, CHCTRL_RELAY_PIN, DIO_HIGH);
    }
    else
    {
        return DIO_u8SetPinValue(CHCTRL_RELAY_PORT, CHCTRL_RELAY_PIN, DIO_LOW);
    }
#else
    if (Copy_u8State == CHCTRL_RELAY_ON)
    {
        return DIO_u8SetPinValue(CHCTRL_RELAY_PORT, CHCTRL_RELAY_PIN, DIO_LOW);
    }
    else
    {
        return DIO_u8SetPinValue(CHCTRL_RELAY_PORT, CHCTRL_RELAY_PIN, DIO_HIGH);
    }
#endif
}