#ifndef CHARGE_CURRENT_DETECT_CONFIG_H
#define CHARGE_CURRENT_DETECT_CONFIG_H

#include "../../MCAL/ADC/ADC_interface.h"

/*
 * Feature switch:
 * 1 -> use shunt current detection
 * 0 -> disable this module logically
 */
#define CCDET_ENABLE_FEATURE          0

/*
 * ADC channel connected to shunt sense point
 * Change this only if you really connected the remaining ADC/input accordingly.
 */
#define CCDET_ADC_CHANNEL             ADC_CHANNEL_AN3

/* ADC reference voltage in mV */
#define CCDET_ADC_VREF_MV             5000UL

/*
 * If using a divider before the ADC, define it here.
 * If no divider exists, use RTOP=0 and RBOTTOM=1
 *
 * Vreal = Vpin * (Rtop + Rbottom) / Rbottom
 */
#define CCDET_RTOP                    0UL
#define CCDET_RBOTTOM                 1UL

/*
 * Threshold in mV across shunt sense point:
 * If measured voltage >= this threshold, consider charging current exists.
 *
 * Start with something simple like 20mV or 30mV depending on your shunt.
 */
#define CCDET_CURRENT_PRESENT_MV      20UL

/*
 * Hysteresis for cleaner decision:
 * charging current becomes absent only if voltage goes below this lower threshold.
 */
#define CCDET_CURRENT_ABSENT_MV       10UL

#endif