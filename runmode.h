/*
 * runmode.h
 *
 *  Created on: 18.10.2019
 *      Author: sebastian
 */

#ifndef RUNMODE_H_
#define RUNMODE_H_
#include <stdint.h>

extern uint8_t lpm_state;
void UART_TX(char * tx_data);

#endif /* RUNMODE_H_ */