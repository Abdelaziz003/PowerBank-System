#ifndef CHARGE_CTRL_INTERFACE_H
#define CHARGE_CTRL_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

#define CHCTRL_OK                       0
#define CHCTRL_NOK                      1

#define CHCTRL_RELAY_OFF                0
#define CHCTRL_RELAY_ON                 1

#define CHCTRL_NO_CHARGER               0
#define CHCTRL_CHARGING_ACTIVE          1
#define CHCTRL_CHARGE_CUTOFF_LATCHED    2

typedef struct
{
    u8 u8ChargeState;
    u8 u8ChargeRelayState;
    u8 u8CutoffLatched;
} CHCTRL_Data_t;

u8 CHCTRL_u8Init(void);
u8 CHCTRL_u8Update(void);
u8 CHCTRL_u8GetData(CHCTRL_Data_t *Copy_pstData);

#endif