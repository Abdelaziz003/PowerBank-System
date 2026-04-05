#ifndef BATTERY_MONITOR_INTERFACE_H
#define BATTERY_MONITOR_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

#define BATMON_OK                    0
#define BATMON_NOK                   1

#define BATMON_RELAY_OFF             0
#define BATMON_RELAY_ON              1

#define BATMON_RELAY_CTRL_DISABLE    0
#define BATMON_RELAY_CTRL_ENABLE     1

typedef struct
{
    u16 u16Tap1_mV;
    u16 u16Tap2_mV;
    u16 u16Tap3_mV;

    u16 u16Cell1_mV;
    u16 u16Cell2_mV;
    u16 u16Cell3_mV;

    u8  u8RelayState;
} BATMON_Data_t;

u8 BATMON_u8Init(void);
u8 BATMON_u8Update(void);
u8 BATMON_u8GetData(BATMON_Data_t *Copy_pstData);

u8 BATMON_u8SetRelayControlEnable(u8 Copy_u8Enable);
u8 BATMON_u8GetRelayControlEnable(u8 *Copy_pu8Enable);

#endif