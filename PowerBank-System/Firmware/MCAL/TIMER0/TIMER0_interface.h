#ifndef TIMER0_INTERFACE_H
#define TIMER0_INTERFACE_H

#include "../../LIB/STD_TYPES.h"

/* Return status */
#define TIMER0_OK              0
#define TIMER0_NOK             1

/* Timer0 mode */
#define TIMER0_TIMER_MODE      0
#define TIMER0_COUNTER_MODE    1

/* Clock source in counter mode */
#define TIMER0_TRANSITION_LOW_TO_HIGH   0
#define TIMER0_TRANSITION_HIGH_TO_LOW   1

/* Prescaler assignment */
#define TIMER0_PRESCALER_TO_WDT         0
#define TIMER0_PRESCALER_TO_TIMER0      1

u8 TIMER0_u8Init(void);
u8 TIMER0_u8SetPreload(u8 Copy_u8PreloadValue);
u8 TIMER0_u8GetTimerValue(u8 *Copy_pu8TimerValue);
u8 TIMER0_u8SetTimerValue(u8 Copy_u8TimerValue);
u8 TIMER0_u8IsOverflowed(u8 *Copy_pu8OverflowFlag);
u8 TIMER0_u8ClearOverflowFlag(void);


/*
 * Interrupt control helpers
 */
u8 TIMER0_u8InterruptEnable(void);
u8 TIMER0_u8InterruptDisable(void);
u8 TIMER0_u8GlobalInterruptEnable(void);
u8 TIMER0_u8GlobalInterruptDisable(void);

#endif