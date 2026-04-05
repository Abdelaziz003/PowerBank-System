#include <xc.h>
#define _XTAL_FREQ 8000000UL

#include "LIB/STD_TYPES.h"
#include "MCAL/ADC/ADC_interface.h"
#include "MCAL/TIMER0/TIMER0_interface.h"

#include "APP/POWERBANK_CTRL/POWERBANK_CTRL_interface.h"
#include "APP/FLASHLIGHT/FLASHLIGHT_interface.h"

/* Configuration Bits */
#pragma config FOSC  = INTOSCIO
#pragma config WDTE  = OFF
#pragma config PWRTE = OFF
#pragma config MCLRE = OFF
#pragma config BOREN = OFF
#pragma config CP    = OFF
#pragma config CPD   = OFF

/*
 * 1 ms system tick flag set by Timer0 ISR.
 * volatile is required because this variable is shared
 * between main context and interrupt context.
 */
volatile u8 g_u8System1msTickFlag = 0;

static void SYSTEM_voidInit(void)
{
    OSCCON = 0b01110001;

    PORTA = 0x00;
    PORTC = 0x00;
}

/*
 * Timer0 interrupt service routine.
 * Keep ISR short:
 * - clear interrupt flag
 * - reload Timer0
 * - raise 1 ms tick flag
 */
void __interrupt() ISR(void)
{
    if (T0IF == 1)
    {
        T0IF = 0;

        /*
         * Timer0 preload for ~1 ms tick.
         * At 8 MHz oscillator:
         * F_instruction = 2 MHz
         * T_instruction = 0.5 us
         * prescaler = 1:32
         * timer increment = 16 us
         * needed counts for ~1 ms = 1000 / 16 ~= 62.5
         * preload ~= 256 - 63 = 193
         */
        TMR0 = 193;

        g_u8System1msTickFlag = 1;
    }
}

void main(void)
{
    
    u8 Local_u8Task10msDivider = 0;

    SYSTEM_voidInit();

    ADC_u8Init();
    TIMER0_u8Init();

    /*
     * Initial preload for ~1 ms tick.
     * Increase this value to make the tick faster.
     * Decrease it to make the tick slower.
     */
    TIMER0_u8SetPreload(193);

    TIMER0_u8InterruptEnable();
    TIMER0_u8GlobalInterruptEnable();

    PBCTRL_u8Init();
    

    while (1)
    {
        /*
         * Run flashlight PWM update every 1 ms.
         * This makes flashlight output smoother than updating it every 10 ms.
         */
        if (g_u8System1msTickFlag == 1)
        {
            g_u8System1msTickFlag = 0;

            /* 1 ms task */
            FLASH_u8Update();

            /*
             * Run the main project controller every 10 ms.
             * Increase divider limit to slow PBCTRL update.
             * Decrease divider limit to speed PBCTRL update.
             */
            Local_u8Task10msDivider++;

            if (Local_u8Task10msDivider >= 10)
            {
                Local_u8Task10msDivider = 0;
                PBCTRL_u8Update();
            }
        }
    }
}