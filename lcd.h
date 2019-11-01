/*
 * lcd.h
 *
 *  Created on: 31.10.2019
 *      Author: sebastian
 */

#pragma once

#define pos1 4                                               // Digit A1 - L4
#define pos2 6                                               // Digit A2 - L6
#define pos3 8                                               // Digit A3 - L8
#define pos4 10                                              // Digit A4 - L10
#define pos5 2                                               // Digit A5 - L2
#define pos6 18                                              // Digit A6 - L18
#define LCDMEMW ((int*)LCDMEM)

extern const char digit[10];

void initLCD();
void Clear_LCD();

