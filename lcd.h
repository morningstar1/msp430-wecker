/*
 * lcd.h
 *
 *  Created on: 31.10.2019
 *      Author: sebastian
 */

#pragma once
#include <stdint.h>
#include <msp430.h>
#define pos1 4                                               // Digit A1 - L4
#define pos2 6                                               // Digit A2 - L6
#define pos3 8                                               // Digit A3 - L8
#define pos4 10                                              // Digit A4 - L10
#define pos5 2                                               // Digit A5 - L2
#define pos6 18                                              // Digit A6 - L18

#define posw1 2                                              // Digit A1 - L4
#define posw2 3                                              // Digit A2 - L6
#define posw3 4                                              // Digit A3 - L8
#define posw4 5                                              // Digit A4 - L10
#define posw5 1                                              // Digit A5 - L2
#define posw6 9                                              // Digit A6 - L18


#define LCDMEMW ((uint16_t*)LCDMEM)

extern const uint16_t digit[10];
extern const uint16_t letter[26];

void initLCD();
void Clear_LCD();

void TestLCD();
