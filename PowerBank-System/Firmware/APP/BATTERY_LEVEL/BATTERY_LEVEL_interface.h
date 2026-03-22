#ifndef BATTERY_LEVEL_INTERFACE_H
#define BATTERY_LEVEL_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

/* Return status */
#define BLEVEL_OK            0
#define BLEVEL_NOK           1

/* Display levels */
#define BLEVEL_0_PERCENT     0
#define BLEVEL_25_PERCENT    25
#define BLEVEL_50_PERCENT    50
#define BLEVEL_75_PERCENT    75
#define BLEVEL_100_PERCENT   100

u8 BLEVEL_u8Init(void);
u8 BLEVEL_u8GetLevel(u8 *Copy_pu8Level);
u8 BLEVEL_u8DisplayLevel(void);
u8 BLEVEL_u8DisplayLevelCharging(u8 Copy_u8BlinkState);
u8 BLEVEL_u8DisplayAllLedsOn(void);
u8 BLEVEL_u8ClearDisplay(void);

#endif