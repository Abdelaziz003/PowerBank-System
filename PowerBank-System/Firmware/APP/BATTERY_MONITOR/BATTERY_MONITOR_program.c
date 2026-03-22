#include "BATTERY_MONITOR_interface.h"
#include "BATTERY_MONITOR_private.h"
#include "BATTERY_MONITOR_config.h"

#include "../../MCAL/ADC/ADC_interface.h"
#include "../../MCAL/DIO/DIO_interface.h"
#include "../CHARGE_CURRENT_DETECT/CHARGE_CURRENT_DETECT_config.h"

static BATMON_Data_t BATMON_stData;

/* Internal counters for filtered relay decision */
static u8 BATMON_u8FaultCounter    = 0;
static u8 BATMON_u8RecoveryCounter = 0;

u8 BATMON_u8Init(void)
{
    u8 Local_u8ErrorState = BATMON_OK;

    Local_u8ErrorState = DIO_u8SetPinDirection(BATMON_RELAY_PORT, BATMON_RELAY_PIN, DIO_OUTPUT);
    DIO_u8SetPinValue(BATMON_RELAY_PORT, BATMON_RELAY_PIN, DIO_LOW);

    BATMON_stData.u16Tap1_mV   = 0;
    BATMON_stData.u16Tap2_mV   = 0;
    BATMON_stData.u16Tap3_mV   = 0;
    BATMON_stData.u16Cell1_mV  = 0;
    BATMON_stData.u16Cell2_mV  = 0;
    BATMON_stData.u16Cell3_mV  = 0;
    BATMON_stData.u8RelayState = BATMON_RELAY_OFF;

    BATMON_u8FaultCounter = 0;
    BATMON_u8RecoveryCounter = 0;

    return Local_u8ErrorState;
}

u8 BATMON_u8Update(void)
{
    u8  Local_u8ErrorState = BATMON_OK;

    u16 Local_u16AdcTap1 = 0;
    u16 Local_u16AdcTap2 = 0;
    u16 Local_u16AdcTap3 = 0;
    u16 Local_u16AdcCurrentSense = 0;

    u32 Local_u32Tap1_Pin_mV = 0;
    u32 Local_u32Tap2_Pin_mV = 0;
    u32 Local_u32Tap3_Pin_mV = 0;
    u32 Local_u32CurrentSense_Pin_mV = 0;

    u32 Local_u32Tap1_Real_mV = 0;
    u32 Local_u32Tap2_Real_mV = 0;
    u32 Local_u32Tap3_Real_mV = 0;
    u32 Local_u32CurrentSense_Real_mV = 0;

    u32 Local_u32Tap1_Corrected_mV = 0;
    u32 Local_u32Tap2_Corrected_mV = 0;
    u32 Local_u32Tap3_Corrected_mV = 0;

    u8 Local_u8AnyCellBelowDisconnect = 0;
    u8 Local_u8AllCellsAboveReconnect = 0;

    /* Read battery tap ADC channels */
    Local_u8ErrorState = ADC_u8GetChannelReading(BATMON_TAP1_CHANNEL, &Local_u16AdcTap1);

    if (Local_u8ErrorState == BATMON_OK)
    {
        Local_u8ErrorState = ADC_u8GetChannelReading(BATMON_TAP2_CHANNEL, &Local_u16AdcTap2);
    }

    if (Local_u8ErrorState == BATMON_OK)
    {
        Local_u8ErrorState = ADC_u8GetChannelReading(BATMON_TAP3_CHANNEL, &Local_u16AdcTap3);
    }

#if (CCDET_ENABLE_FEATURE == 1)
    if (Local_u8ErrorState == BATMON_OK)
    {
        Local_u8ErrorState = ADC_u8GetChannelReading(BATMON_CURRENT_SENSE_CHANNEL, &Local_u16AdcCurrentSense);
    }
#else
    Local_u16AdcCurrentSense = 0;
#endif

    if (Local_u8ErrorState == BATMON_OK)
    {
        /* Convert ADC readings to ADC-pin voltages */
        Local_u32Tap1_Pin_mV = BATMON_u32ConvertAdcToPinMilliVolt(Local_u16AdcTap1);
        Local_u32Tap2_Pin_mV = BATMON_u32ConvertAdcToPinMilliVolt(Local_u16AdcTap2);
        Local_u32Tap3_Pin_mV = BATMON_u32ConvertAdcToPinMilliVolt(Local_u16AdcTap3);

#if (CCDET_ENABLE_FEATURE == 1)
        Local_u32CurrentSense_Pin_mV = BATMON_u32ConvertAdcToPinMilliVolt(Local_u16AdcCurrentSense);
#else
        Local_u32CurrentSense_Pin_mV = 0;
#endif

        /* Recover actual voltages before dividers */
        Local_u32Tap1_Real_mV = BATMON_u32RecoverOriginalMilliVolt(Local_u32Tap1_Pin_mV, BATMON_TAP1_RTOP, BATMON_TAP1_RBOTTOM);
        Local_u32Tap2_Real_mV = BATMON_u32RecoverOriginalMilliVolt(Local_u32Tap2_Pin_mV, BATMON_TAP2_RTOP, BATMON_TAP2_RBOTTOM);
        Local_u32Tap3_Real_mV = BATMON_u32RecoverOriginalMilliVolt(Local_u32Tap3_Pin_mV, BATMON_TAP3_RTOP, BATMON_TAP3_RBOTTOM);

#if (CCDET_ENABLE_FEATURE == 1)
        Local_u32CurrentSense_Real_mV = BATMON_u32RecoverOriginalMilliVolt(Local_u32CurrentSense_Pin_mV, BATMON_CS_RTOP, BATMON_CS_RBOTTOM);
#else
        Local_u32CurrentSense_Real_mV = 0;
#endif

        /* Correct taps only if shunt feature is enabled */
#if (CCDET_ENABLE_FEATURE == 1)
        if (Local_u32Tap1_Real_mV > Local_u32CurrentSense_Real_mV)
        {
            Local_u32Tap1_Corrected_mV = Local_u32Tap1_Real_mV - Local_u32CurrentSense_Real_mV;
        }
        else
        {
            Local_u32Tap1_Corrected_mV = 0;
        }

        if (Local_u32Tap2_Real_mV > Local_u32CurrentSense_Real_mV)
        {
            Local_u32Tap2_Corrected_mV = Local_u32Tap2_Real_mV - Local_u32CurrentSense_Real_mV;
        }
        else
        {
            Local_u32Tap2_Corrected_mV = 0;
        }

        if (Local_u32Tap3_Real_mV > Local_u32CurrentSense_Real_mV)
        {
            Local_u32Tap3_Corrected_mV = Local_u32Tap3_Real_mV - Local_u32CurrentSense_Real_mV;
        }
        else
        {
            Local_u32Tap3_Corrected_mV = 0;
        }
#else
        Local_u32Tap1_Corrected_mV = Local_u32Tap1_Real_mV;
        Local_u32Tap2_Corrected_mV = Local_u32Tap2_Real_mV;
        Local_u32Tap3_Corrected_mV = Local_u32Tap3_Real_mV;
#endif

        BATMON_stData.u16Tap1_mV = (u16)Local_u32Tap1_Corrected_mV;
        BATMON_stData.u16Tap2_mV = (u16)Local_u32Tap2_Corrected_mV;
        BATMON_stData.u16Tap3_mV = (u16)Local_u32Tap3_Corrected_mV;

        /* Extract individual cell voltages */
        BATMON_stData.u16Cell1_mV = BATMON_stData.u16Tap1_mV;

        if (BATMON_stData.u16Tap2_mV >= BATMON_stData.u16Tap1_mV)
        {
            BATMON_stData.u16Cell2_mV = BATMON_stData.u16Tap2_mV - BATMON_stData.u16Tap1_mV;
        }
        else
        {
            BATMON_stData.u16Cell2_mV = 0;
        }

        if (BATMON_stData.u16Tap3_mV >= BATMON_stData.u16Tap2_mV)
        {
            BATMON_stData.u16Cell3_mV = BATMON_stData.u16Tap3_mV - BATMON_stData.u16Tap2_mV;
        }
        else
        {
            BATMON_stData.u16Cell3_mV = 0;
        }

        /* Threshold checks */
        if ((BATMON_stData.u16Cell1_mV <= BATMON_CELL_DISCONNECT_MV) ||
            (BATMON_stData.u16Cell2_mV <= BATMON_CELL_DISCONNECT_MV) ||
            (BATMON_stData.u16Cell3_mV <= BATMON_CELL_DISCONNECT_MV))
        {
            Local_u8AnyCellBelowDisconnect = 1;
        }

        if ((BATMON_stData.u16Cell1_mV >= BATMON_CELL_RECONNECT_MV) &&
            (BATMON_stData.u16Cell2_mV >= BATMON_CELL_RECONNECT_MV) &&
            (BATMON_stData.u16Cell3_mV >= BATMON_CELL_RECONNECT_MV))
        {
            Local_u8AllCellsAboveReconnect = 1;
        }

        /* Relay logic with hysteresis + filtering */
        if (BATMON_stData.u8RelayState == BATMON_RELAY_ON)
        {
            if (Local_u8AnyCellBelowDisconnect == 1)
            {
                if (BATMON_u8FaultCounter < BATMON_FAULT_COUNT_LIMIT)
                {
                    BATMON_u8FaultCounter++;
                }

                BATMON_u8RecoveryCounter = 0;

                if (BATMON_u8FaultCounter >= BATMON_FAULT_COUNT_LIMIT)
                {
                    BATMON_stData.u8RelayState = BATMON_RELAY_OFF;
                    DIO_u8SetPinValue(BATMON_RELAY_PORT, BATMON_RELAY_PIN, DIO_LOW);
                }
            }
            else
            {
                BATMON_u8FaultCounter = 0;
                BATMON_stData.u8RelayState = BATMON_RELAY_ON;
                DIO_u8SetPinValue(BATMON_RELAY_PORT, BATMON_RELAY_PIN, DIO_HIGH);
            }
        }
        else
        {
            if (Local_u8AllCellsAboveReconnect == 1)
            {
                if (BATMON_u8RecoveryCounter < BATMON_RECOVERY_COUNT_LIMIT)
                {
                    BATMON_u8RecoveryCounter++;
                }

                BATMON_u8FaultCounter = 0;

                if (BATMON_u8RecoveryCounter >= BATMON_RECOVERY_COUNT_LIMIT)
                {
                    BATMON_stData.u8RelayState = BATMON_RELAY_ON;
                    DIO_u8SetPinValue(BATMON_RELAY_PORT, BATMON_RELAY_PIN, DIO_HIGH);
                }
            }
            else
            {
                BATMON_u8RecoveryCounter = 0;
                BATMON_stData.u8RelayState = BATMON_RELAY_OFF;
                DIO_u8SetPinValue(BATMON_RELAY_PORT, BATMON_RELAY_PIN, DIO_LOW);
            }
        }
    }

    return Local_u8ErrorState;
}

u8 BATMON_u8GetData(BATMON_Data_t *Copy_pstData)
{
    if (Copy_pstData == 0)
    {
        return BATMON_NOK;
    }

    *Copy_pstData = BATMON_stData;
    return BATMON_OK;
}

static u32 BATMON_u32ConvertAdcToPinMilliVolt(u16 Copy_u16AdcReading)
{
    return ((u32)Copy_u16AdcReading * BATMON_ADC_VREF_MV) / 1023UL;
}

static u32 BATMON_u32RecoverOriginalMilliVolt(u32 Copy_u32PinMilliVolt, u32 Copy_u32RTop, u32 Copy_u32RBottom)
{
    if (Copy_u32RBottom == 0UL)
    {
        return 0UL;
    }

    return (Copy_u32PinMilliVolt * (Copy_u32RTop + Copy_u32RBottom)) / Copy_u32RBottom;
}