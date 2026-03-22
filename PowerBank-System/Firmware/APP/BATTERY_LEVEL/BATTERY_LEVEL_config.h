#ifndef BATTERY_LEVEL_CONFIG_H
#define BATTERY_LEVEL_CONFIG_H

#include "../../MCAL/DIO/DIO_interface.h"

/* LED pins */
#define BLEVEL_LED25_PORT         DIO_PORTA
#define BLEVEL_LED25_PIN          DIO_PIN0

#define BLEVEL_LED50_PORT         DIO_PORTA
#define BLEVEL_LED50_PIN          DIO_PIN1

#define BLEVEL_LED75_PORT         DIO_PORTA
#define BLEVEL_LED75_PIN          DIO_PIN2

#define BLEVEL_LED100_PORT        DIO_PORTC
#define BLEVEL_LED100_PIN         DIO_PIN0

/*
 * Pack voltage thresholds in mV (3S lithium-ion)
 * Simple display thresholds
 */
#define BLEVEL_PACK_25_MV         9900UL
#define BLEVEL_PACK_50_MV         10800UL
#define BLEVEL_PACK_75_MV         11400UL
#define BLEVEL_PACK_100_MV        12000UL

#endif