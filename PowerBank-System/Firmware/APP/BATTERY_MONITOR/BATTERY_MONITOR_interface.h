#ifndef BATTERY_MONITOR_INTERFACE_H
#define BATTERY_MONITOR_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

/* Return status */
#define BATMON_OK        0
#define BATMON_NOK       1

/* Relay states */
#define BATMON_RELAY_OFF 0
#define BATMON_RELAY_ON  1

typedef struct
{
    u16 u16Tap1_mV;   /* Voltage of VB1 tap */
    u16 u16Tap2_mV;   /* Voltage of VB1+VB2 tap */
    u16 u16Tap3_mV;   /* Voltage of VB1+VB2+VB3 tap */

    u16 u16Cell1_mV;  /* Cell 1 voltage */
    u16 u16Cell2_mV;  /* Cell 2 voltage */
    u16 u16Cell3_mV;  /* Cell 3 voltage */

    u8  u8RelayState; /* Relay ON / OFF */
} BATMON_Data_t;

/*
 * Function: BATMON_u8Init
 * -----------------------
 * Initializes relay output pin.
 */
u8 BATMON_u8Init(void);

/*
 * Function: BATMON_u8Update
 * -------------------------
 * Reads ADC channels, calculates tap and cell voltages,
 * then updates relay state based on protection threshold.
 */
u8 BATMON_u8Update(void);

/*
 * Function: BATMON_u8GetData
 * --------------------------
 * Returns the latest measured battery data.
 */
u8 BATMON_u8GetData(BATMON_Data_t *Copy_pstData);

#endif