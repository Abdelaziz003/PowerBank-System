#ifndef POWERBANK_CTRL_INTERFACE_H
#define POWERBANK_CTRL_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

#define PBCTRL_OK   0
#define PBCTRL_NOK  1

u8 PBCTRL_u8Init(void);
u8 PBCTRL_u8Update(void);

#endif