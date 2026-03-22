#include "ADC_interface.h"
#include "ADC_private.h"
#include "ADC_config.h"
#include "../../LIB/BIT_MATH.h"

#include <xc.h>
#define _XTAL_FREQ 8000000UL

static u8 ADC_u8IsValidChannel(u8 Copy_u8Channel);

u8 ADC_u8Init(void)
{
    u8 Local_u8ErrorState = ADC_OK;

    /* Disable ADC before configuration */
    ADCON0bits.ADON = 0;

    /*
     * Configure analog pins.
     * For simplicity, all ADC-capable pins are enabled as analog.
     * Later we can reduce this to only the channels we use.
     */
    ANSEL = 0xE8;

    /* Disable comparators to avoid conflicts with shared pins */
    CMCON0 = 0x07;

    /*
     * ADCON0 bits on PIC16F688:
     * bit7 = ADFM
     * bit6 = VCFG
     * bit4:2 = CHS<2:0>
     * bit1 = GO/DONE
     * bit0 = ADON
     */

    /* Configure result format */
#if (ADC_RESULT_FORMAT_CONFIG == ADC_RIGHT_JUSTIFIED)
    ADCON0bits.ADFM = 1;
#elif (ADC_RESULT_FORMAT_CONFIG == ADC_LEFT_JUSTIFIED)
    ADCON0bits.ADFM = 0;
#else
    Local_u8ErrorState = ADC_NOK;
#endif

    /* Configure voltage reference */
#if (ADC_VREF_CONFIG == ADC_VREF_VDD)
    ADCON0bits.VCFG = 0;
#elif (ADC_VREF_CONFIG == ADC_VREF_EXTERNAL)
    ADCON0bits.VCFG = 1;
#else
    Local_u8ErrorState = ADC_NOK;
#endif

    /*
     * ADCON1 bits on PIC16F688:
     * ADCS<2:0> are bits 6:4
     */
    ADCON1 &= 0b10001111;
    ADCON1 |= ((ADC_CLOCK_CONFIG & 0x07) << 4);

    /* Default channel = AN0 */
    ADCON0bits.CHS = 0;

    /* Enable ADC */
    ADCON0bits.ADON = 1;

    return Local_u8ErrorState;
}

u8 ADC_u8SelectChannel(u8 Copy_u8Channel)
{
    u8 Local_u8ErrorState = ADC_OK;

    if (ADC_u8IsValidChannel(Copy_u8Channel) == 0)
    {
        Local_u8ErrorState = ADC_NOK;
    }
    else
    {
        ADCON0bits.CHS = Copy_u8Channel;

        /* Acquisition delay after channel switching */
        __delay_us(ADC_ACQUISITION_DELAY_US);
    }

    return Local_u8ErrorState;
}

u8 ADC_u8StartConversion(void)
{
    /*
     * Note:
     * GO/DONE should not be set in the same instruction
     * that turns ADC on.
     */
    ADCON0bits.GO_DONE = 1;
    return ADC_OK;
}

u8 ADC_u8IsConversionDone(u8 *Copy_pu8Status)
{
    if (Copy_pu8Status == NULL)
    {
        return ADC_NOK;
    }

    if (ADCON0bits.GO_DONE == 0)
    {
        *Copy_pu8Status = 1;
    }
    else
    {
        *Copy_pu8Status = 0;
    }

    return ADC_OK;
}

u8 ADC_u8GetResult(u16 *Copy_pu16Result)
{
    if (Copy_pu16Result == NULL)
    {
        return ADC_NOK;
    }

#if (ADC_RESULT_FORMAT_CONFIG == ADC_RIGHT_JUSTIFIED)
    *Copy_pu16Result = (((u16)ADRESH) << 8) | ADRESL;
#elif (ADC_RESULT_FORMAT_CONFIG == ADC_LEFT_JUSTIFIED)
    *Copy_pu16Result = ((((u16)ADRESH) << 8) | ADRESL) >> 6;
#else
    return ADC_NOK;
#endif

    return ADC_OK;
}

u8 ADC_u8GetChannelReading(u8 Copy_u8Channel, u16 *Copy_pu16Result)
{
    u8 Local_u8Done = 0;
    u8 Local_u8ErrorState = ADC_OK;

    if (Copy_pu16Result == NULL)
    {
        return ADC_NOK;
    }

    Local_u8ErrorState = ADC_u8SelectChannel(Copy_u8Channel);

    if (Local_u8ErrorState == ADC_OK)
    {
        Local_u8ErrorState = ADC_u8StartConversion();
    }

    if (Local_u8ErrorState == ADC_OK)
    {
        do
        {
            Local_u8ErrorState = ADC_u8IsConversionDone(&Local_u8Done);
        }
        while ((Local_u8Done == 0) && (Local_u8ErrorState == ADC_OK));
    }

    if (Local_u8ErrorState == ADC_OK)
    {
        Local_u8ErrorState = ADC_u8GetResult(Copy_pu16Result);
    }

    return Local_u8ErrorState;
}

static u8 ADC_u8IsValidChannel(u8 Copy_u8Channel)
{
    if (Copy_u8Channel <= ADC_MAX_CHANNEL_NUMBER)
    {
        return 1;
    }
    return 0;
}