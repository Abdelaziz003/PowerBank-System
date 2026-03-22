#ifndef CHARGE_CURRENT_DETECT_INTERFACE_H
#define CHARGE_CURRENT_DETECT_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

#define CCDET_OK                0
#define CCDET_NOK               1

#define CCDET_NO_CHARGE_CURRENT 0
#define CCDET_CHARGE_CURRENT    1

u8 CCDET_u8Init(void);
u8 CCDET_u8Update(void);
u8 CCDET_u8GetState(u8 *Copy_pu8State);

#endif