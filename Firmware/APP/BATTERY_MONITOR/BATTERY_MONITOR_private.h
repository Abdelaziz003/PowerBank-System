#ifndef BATTERY_MONITOR_PRIVATE_H
#define BATTERY_MONITOR_PRIVATE_H

#include "../../LIB/STD_TYPES.h"

static u32 BATMON_u32ConvertAdcToPinMilliVolt(u16 Copy_u16AdcReading);
static u32 BATMON_u32RecoverOriginalMilliVolt(u32 Copy_u32PinMilliVolt, u32 Copy_u32RTop, u32 Copy_u32RBottom);

#endif