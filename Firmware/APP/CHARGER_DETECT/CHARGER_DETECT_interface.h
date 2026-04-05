#ifndef CHARGER_DETECT_INTERFACE_H
#define CHARGER_DETECT_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

#define CHGDET_OK              0
#define CHGDET_NOK             1

#define CHGDET_NOT_CONNECTED   0
#define CHGDET_CONNECTED       1

u8 CHGDET_u8Init(void);
u8 CHGDET_u8Update(void);
u8 CHGDET_u8GetState(u8 *Copy_pu8State);

#endif