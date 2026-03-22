#ifndef ADC_CONFIG_H
#define ADC_CONFIG_H

/* Select voltage reference */
#define ADC_VREF_CONFIG            ADC_VREF_VDD

/* Select result alignment */
#define ADC_RESULT_FORMAT_CONFIG   ADC_RIGHT_JUSTIFIED

/*
 * ADCS<2:0> options for PIC16F688:
 * 000 -> FOSC/2
 * 100 -> FOSC/4
 * 001 -> FOSC/8
 * 101 -> FOSC/16
 * 010 -> FOSC/32
 * 110 -> FOSC/64
 * 111 -> FRC
 *
 * At 8 MHz, FOSC/32 is a safe simple choice.
 */
#define ADC_CLOCK_CONFIG           0b010

/* Acquisition delay after channel switching */
#define ADC_ACQUISITION_DELAY_US   50

#endif