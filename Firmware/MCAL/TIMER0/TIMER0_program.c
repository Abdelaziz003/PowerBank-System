#include "TIMER0_interface.h"
#include "TIMER0_private.h"
#include "TIMER0_config.h"
#include "../../LIB/BIT_MATH.h"

u8 TIMER0_u8Init(void)
{
    u8 Local_u8ErrorState = TIMER0_OK;

    /* Clear Timer0 interrupt flag */
    CLR_BIT(INTCON, TIMER0_T0IF_BIT);

    /* Configure mode */
#if (TIMER0_MODE_CONFIG == TIMER0_TIMER_MODE)
    CLR_BIT(OPTION_REG, TIMER0_T0CS_BIT);
#elif (TIMER0_MODE_CONFIG == TIMER0_COUNTER_MODE)
    SET_BIT(OPTION_REG, TIMER0_T0CS_BIT);
#else
    Local_u8ErrorState = TIMER0_NOK;
#endif

    /* Configure edge in counter mode */
#if (TIMER0_COUNTER_EDGE_CONFIG == TIMER0_TRANSITION_LOW_TO_HIGH)
    CLR_BIT(OPTION_REG, TIMER0_T0SE_BIT);
#elif (TIMER0_COUNTER_EDGE_CONFIG == TIMER0_TRANSITION_HIGH_TO_LOW)
    SET_BIT(OPTION_REG, TIMER0_T0SE_BIT);
#else
    Local_u8ErrorState = TIMER0_NOK;
#endif

    /* Configure prescaler assignment */
#if (TIMER0_PRESCALER_ASSIGN_CONFIG == TIMER0_PRESCALER_TO_WDT)
    SET_BIT(OPTION_REG, TIMER0_PSA_BIT);
#elif (TIMER0_PRESCALER_ASSIGN_CONFIG == TIMER0_PRESCALER_TO_TIMER0)
    CLR_BIT(OPTION_REG, TIMER0_PSA_BIT);

    /* Set PS2:PS0 */
    OPTION_REG &= 0b11111000;
    OPTION_REG |= (TIMER0_PRESCALER_VALUE_CONFIG & 0x07);
#else
    Local_u8ErrorState = TIMER0_NOK;
#endif

    return Local_u8ErrorState;
}

u8 TIMER0_u8SetPreload(u8 Copy_u8PreloadValue)
{
    TMR0 = Copy_u8PreloadValue;
    return TIMER0_OK;
}

u8 TIMER0_u8GetTimerValue(u8 *Copy_pu8TimerValue)
{
    if (Copy_pu8TimerValue == 0)
    {
        return TIMER0_NOK;
    }

    *Copy_pu8TimerValue = TMR0;
    return TIMER0_OK;
}

u8 TIMER0_u8SetTimerValue(u8 Copy_u8TimerValue)
{
    TMR0 = Copy_u8TimerValue;
    return TIMER0_OK;
}

u8 TIMER0_u8IsOverflowed(u8 *Copy_pu8OverflowFlag)
{
    if (Copy_pu8OverflowFlag == 0)
    {
        return TIMER0_NOK;
    }

    *Copy_pu8OverflowFlag = GET_BIT(INTCON, TIMER0_T0IF_BIT);
    return TIMER0_OK;
}

u8 TIMER0_u8ClearOverflowFlag(void)
{
    CLR_BIT(INTCON, TIMER0_T0IF_BIT);
    return TIMER0_OK;
}

u8 TIMER0_u8InterruptEnable(void)
{
    SET_BIT(INTCON, TIMER0_T0IE_BIT);
    return TIMER0_OK;
}

u8 TIMER0_u8InterruptDisable(void)
{
    CLR_BIT(INTCON, TIMER0_T0IE_BIT);
    return TIMER0_OK;
}

u8 TIMER0_u8GlobalInterruptEnable(void)
{
    SET_BIT(INTCON, TIMER0_GIE_BIT);
    return TIMER0_OK;
}

u8 TIMER0_u8GlobalInterruptDisable(void)
{
    CLR_BIT(INTCON, TIMER0_GIE_BIT);
    return TIMER0_OK;
}