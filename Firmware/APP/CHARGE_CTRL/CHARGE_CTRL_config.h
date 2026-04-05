#ifndef CHARGE_CTRL_CONFIG_H
#define CHARGE_CTRL_CONFIG_H

#include "../../MCAL/DIO/DIO_interface.h"

/*
 * Charge relay output pin
 * ????? ??? ???????? ????
 */
#define CHCTRL_RELAY_PORT           DIO_PORTA
#define CHCTRL_RELAY_PIN            DIO_PIN4

/*
 * Relay output polarity
 * ??? HIGH ???? ??????? ???? 1
 * ??? LOW ???? ??????? ???? 0
 */
#define CHCTRL_RELAY_ACTIVE_HIGH    1U
#define CHCTRL_RELAY_ACTIVE_LOW     0U

#define CHCTRL_RELAY_POLARITY       CHCTRL_RELAY_ACTIVE_HIGH

/*
 * Any cell reaching this voltage or above
 * causes charge cutoff latch for this charger session.
 */
#define CHCTRL_CELL_CUTOFF_MV       4250U

#endif