#ifndef CHARGER_DETECT_CONFIG_H
#define CHARGER_DETECT_CONFIG_H

#include "../../MCAL/DIO/DIO_interface.h"

#define CHGDET_PORT            DIO_PORTA
#define CHGDET_PIN             DIO_PIN5

/* Electrical meaning */
#define CHGDET_ACTIVE_LOW      0
#define CHGDET_ACTIVE_HIGH     1

#define CHGDET_ACTIVE_MODE     CHGDET_ACTIVE_LOW

#endif