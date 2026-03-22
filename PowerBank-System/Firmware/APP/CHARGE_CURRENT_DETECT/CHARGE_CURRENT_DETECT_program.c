#include "CHARGE_CURRENT_DETECT_interface.h"
#include "CHARGE_CURRENT_DETECT_private.h"
#include "CHARGE_CURRENT_DETECT_config.h"

#include "../../MCAL/ADC/ADC_interface.h"

static u8 CCDET_u8State = CCDET_NO_CHARGE_CURRENT;

static u32 CCDET_u32ConvertAdcToPinMilliVolt(u16 Copy_u16AdcReading)
{
    return ((u32)Copy_u16AdcReading * CCDET_ADC_VREF_MV) / 1023UL;
}

static u32 CCDET_u32RecoverOriginalMilliVolt(u32 Copy_u32PinMilliVolt)
{
    if (CCDET_RBOTTOM == 0UL)
    {
        return 0UL;
    }

    return (Copy_u32PinMilliVolt * (CCDET_RTOP + CCDET_RBOTTOM)) / CCDET_RBOTTOM;
}

u8 CCDET_u8Init(void)
{
#if (CCDET_ENABLE_FEATURE == 1)
    CCDET_u8State = CCDET_NO_CHARGE_CURRENT;
#else
    CCDET_u8State = CCDET_NO_CHARGE_CURRENT;
#endif
    return CCDET_OK;
}

u8 CCDET_u8Update(void)
{
#if (CCDET_ENABLE_FEATURE == 1)
    u16 Local_u16AdcReading = 0;
    u32 Local_u32PinMilliVolt = 0;
    u32 Local_u32RealMilliVolt = 0;

    if (ADC_u8GetChannelReading(CCDET_ADC_CHANNEL, &Local_u16AdcReading) != ADC_OK)
    {
        return CCDET_NOK;
    }

    Local_u32PinMilliVolt = CCDET_u32ConvertAdcToPinMilliVolt(Local_u16AdcReading);
    Local_u32RealMilliVolt = CCDET_u32RecoverOriginalMilliVolt(Local_u32PinMilliVolt);

    /*
     * Hysteresis:
     * If current is already considered present, keep it present
     * until voltage drops below CURRENT_ABSENT_MV.
     *
     * If current is absent, only declare present
     * when voltage rises above CURRENT_PRESENT_MV.
     */
    if (CCDET_u8State == CCDET_CHARGE_CURRENT)
    {
        if (Local_u32RealMilliVolt <= CCDET_CURRENT_ABSENT_MV)
        {
            CCDET_u8State = CCDET_NO_CHARGE_CURRENT;
        }
    }
    else
    {
        if (Local_u32RealMilliVolt >= CCDET_CURRENT_PRESENT_MV)
        {
            CCDET_u8State = CCDET_CHARGE_CURRENT;
        }
    }

#else
    /* Feature disabled -> always behave as if no current detection exists */
    CCDET_u8State = CCDET_NO_CHARGE_CURRENT;
#endif

    return CCDET_OK;
}

u8 CCDET_u8GetState(u8 *Copy_pu8State)
{
    if (Copy_pu8State == 0)
    {
        return CCDET_NOK;
    }

    *Copy_pu8State = CCDET_u8State;
    return CCDET_OK;
}