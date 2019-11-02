/*
 * runmode.h
 *
 *  Created on: 18.10.2019
 *      Author: sebastian
 */

#pragma once
#include <stdint.h>

extern uint8_t lpm_state;
extern uint8_t playsound;
extern uint8_t dcfCaptureRunning;

#define EXIT_ISR { switch (lpm_state) {\
case 0:\
    LPM0_EXIT;\
    break;\
case 3:\
    LPM3_EXIT;\
    break;\
case 35:\
    LPM3_EXIT;\
    break;\
case 4:\
    LPM4_EXIT;\
    break;\
}\
}
