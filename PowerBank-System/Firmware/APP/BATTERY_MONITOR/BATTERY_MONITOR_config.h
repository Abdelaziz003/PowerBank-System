#ifndef BATTERY_MONITOR_CONFIG_H
#define BATTERY_MONITOR_CONFIG_H

#include "../../MCAL/DIO/DIO_interface.h"
#include "../../MCAL/ADC/ADC_interface.h"

/* ADC channels */
#define BATMON_TAP1_CHANNEL              ADC_CHANNEL_AN5   /* RC1 -> VB1 */
#define BATMON_TAP2_CHANNEL              ADC_CHANNEL_AN6   /* RC2 -> VB1+VB2 */
#define BATMON_TAP3_CHANNEL              ADC_CHANNEL_AN7   /* RC3 -> VB1+VB2+VB3 */

/* Current sense ADC channel (low-side shunt offset) */
#define BATMON_CURRENT_SENSE_CHANNEL     ADC_CHANNEL_AN3   /* RA4 -> current_sense */

/* Relay pin */
#define BATMON_RELAY_PORT                DIO_PORTC
#define BATMON_RELAY_PIN                 DIO_PIN5

/* ADC reference voltage in mV */
#define BATMON_ADC_VREF_MV               5000UL

/* Undervoltage hysteresis thresholds */
#define BATMON_CELL_DISCONNECT_MV        3200UL
#define BATMON_CELL_RECONNECT_MV         3550UL

/* Decision filters */
#define BATMON_FAULT_COUNT_LIMIT         3U
#define BATMON_RECOVERY_COUNT_LIMIT      3U

/* Divider values for taps */
#define BATMON_TAP1_RTOP                 100000UL
#define BATMON_TAP1_RBOTTOM              47000UL

#define BATMON_TAP2_RTOP                 100000UL
#define BATMON_TAP2_RBOTTOM              47000UL

#define BATMON_TAP3_RTOP                 100000UL
#define BATMON_TAP3_RBOTTOM              47000UL

/* Current sense path divider
   If current_sense is connected directly to ADC pin:
   RTOP = 0, RBOTTOM = 1
*/
#define BATMON_CS_RTOP                   0UL
#define BATMON_CS_RBOTTOM                1UL

#endif