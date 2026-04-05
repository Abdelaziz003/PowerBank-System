#ifndef FLASHLIGHT_INTERFACE_H
#define FLASHLIGHT_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

#define FLASH_OK        0
#define FLASH_NOK       1

#define FLASH_OFF       0
#define FLASH_ON        1

u8 FLASH_u8Init(void);
u8 FLASH_u8Toggle(void);
u8 FLASH_u8SetState(u8 Copy_u8State);
u8 FLASH_u8GetState(u8 *Copy_pu8State);
u8 FLASH_u8Update(void);

#endif