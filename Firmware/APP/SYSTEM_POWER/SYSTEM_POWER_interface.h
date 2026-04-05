#ifndef SYSTEM_POWER_INTERFACE_H
#define SYSTEM_POWER_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

/* Return status */
#define SPWR_OK                 0
#define SPWR_NOK                1

/* System states */
#define SPWR_SLEEP              0
#define SPWR_STARTUP            1
#define SPWR_ACTIVE             2

typedef struct
{
    u8  u8SystemState;
    u8  u8SleepRequest;
    u8  u8WakeRequest;
    u8  u8StartupActive;
} SPWR_Data_t;

/*
 * Initialize power-state driver.
 */
u8 SPWR_u8Init(void);

/*
 * Update power-state logic using external commands.
 *
 * Copy_u8SleepCommand:
 * 1 -> request sleep
 * 0 -> no sleep request
 *
 * Copy_u8WakeCommand:
 * 1 -> request wake
 * 0 -> no wake request
 *
 * Copy_u8ChargerState:
 * charger detect state used for automatic wake from sleep
 */
u8 SPWR_u8Update(u8 Copy_u8SleepCommand, u8 Copy_u8WakeCommand, u8 Copy_u8ChargerState);

/*
 * Get current state data.
 */
u8 SPWR_u8GetData(SPWR_Data_t *Copy_pstData);

#endif