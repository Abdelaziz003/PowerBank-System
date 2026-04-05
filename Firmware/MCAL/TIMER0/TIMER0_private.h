#ifndef TIMER0_PRIVATE_H
#define TIMER0_PRIVATE_H

#include <xc.h>

/* OPTION_REG bits */
#define TIMER0_PSA_BIT      3
#define TIMER0_T0SE_BIT     4
#define TIMER0_T0CS_BIT     5

/* INTCON bits */
#define TIMER0_T0IF_BIT     2
#define TIMER0_T0IE_BIT     5
#define TIMER0_GIE_BIT      7

#endif