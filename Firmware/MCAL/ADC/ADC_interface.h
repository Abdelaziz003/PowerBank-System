#ifndef ADC_INTERFACE_H
#define ADC_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

/* ADC channels for PIC16F688 */
#define ADC_CHANNEL_AN0      0
#define ADC_CHANNEL_AN1      1
#define ADC_CHANNEL_AN2      2
#define ADC_CHANNEL_AN3      3
#define ADC_CHANNEL_AN4      4
#define ADC_CHANNEL_AN5      5
#define ADC_CHANNEL_AN6      6
#define ADC_CHANNEL_AN7      7

/* Voltage reference selection */
#define ADC_VREF_VDD         0
#define ADC_VREF_EXTERNAL    1

/* Result format */
#define ADC_RIGHT_JUSTIFIED  0
#define ADC_LEFT_JUSTIFIED   1

/* Return status */
#define ADC_OK               0
#define ADC_NOK              1

/*
 * Function: ADC_u8Init
 * --------------------
 * Initializes ADC peripheral based on configuration options.
 */
u8 ADC_u8Init(void);

/*
 * Function: ADC_u8SelectChannel
 * -----------------------------
 * Selects ADC input channel.
 */
u8 ADC_u8SelectChannel(u8 Copy_u8Channel);

/*
 * Function: ADC_u8StartConversion
 * -------------------------------
 * Starts ADC conversion on the selected channel.
 */
u8 ADC_u8StartConversion(void);

/*
 * Function: ADC_u8IsConversionDone
 * --------------------------------
 * 0 -> busy
 * 1 -> finished
 */
u8 ADC_u8IsConversionDone(u8 *Copy_pu8Status);

/*
 * Function: ADC_u8GetResult
 * -------------------------
 * Reads ADC result.
 */
u8 ADC_u8GetResult(u16 *Copy_pu16Result);

/*
 * Function: ADC_u8GetChannelReading
 * ---------------------------------
 * Blocking read:
 * select channel -> start conversion -> wait -> return result
 */
u8 ADC_u8GetChannelReading(u8 Copy_u8Channel, u16 *Copy_pu16Result);

#endif