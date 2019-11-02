/*
 * lcd.c
 *
 *  Created on: 31.10.2019
 *      Author: sebastian
 */
#include <msp430.h>
#include "lcd.h"
#include <stdint.h>

const uint16_t digit[10] ={
    0xFC,                                                    // "0"
    0x60,                                                    // "1"
    0xDB,                                                    // "2"
    0xF3,                                                    // "3"
    0x67,                                                    // "4"
    0xB7,                                                    // "5"
    0xBF,                                                    // "6"
    0xE0,                                                    // "7"
    0xFF,                                                    // "8"
    0xF7                                                     // "9"
};

const uint16_t letter[26]= {
    0xEF, //A
    0xFF, //B
    0x9C, //C
    0xFC, //D
    0x9F, //E
    0x8F, //F
    0xBD, //G
    0x6F, //H
    0x5090, //I
    0x70, //J
    0x220E, //K
    0x1C, //L
    0xA06C, //M
    0x826C, //N
    0xFC, //O
    0xCF, //P
    0x2FC, //Q
    0x2CF, //R
    0xB7, //S
    0x5080, //T
    0x7C, //U
    0x280C, //V
    0xA6C, //w
    0xAA00, //X
    0xB000, //Y
    0x2890, //Z
};

void initLCD(){
    // Configure LCD pins
    SYSCFG2 |= LCDPCTL;                                 // R13/R23/R33/LCDCAP0/LCDCAP1 pins selected

    LCDPCTL0 = 0xFFFF;
    LCDPCTL1 = 0x07FF;
    LCDPCTL2 = 0x00F0;
    LCDCTL0 = LCDSSEL_0 | LCDDIV_7;                     // flcd ref freq is xtclk
    // LCD Operation - Mode 3, internal 3.08v, charge pump 256Hz
    LCDVCTL = LCDCPEN | LCDREFEN | VLCD_5 | (LCDCPFSEL0 | LCDCPFSEL1 | LCDCPFSEL2 | LCDCPFSEL3);

    LCDMEMCTL |= LCDCLRM;                               // Clear LCD memory

    LCDCSSEL0 = 0x000F;                                 // Configure COMs and SEGs
    LCDCSSEL1 = 0x0000;                                 // L0, L1, L2, L3: COM pins
    LCDCSSEL2 = 0x0000;
    LCDM0 = 0x21;                                       // L0 = COM0, L1 = COM1
    LCDM1 = 0x84;                                       // L2 = COM2, L3 = COM3

    LCDCTL0 |= LCD4MUX | LCDON;                         // Turn on LCD, 4-mux selected (LCD4MUX also includes LCDSON)
    Clear_LCD();
}

void Clear_LCD(){
    for(int i=5;i;i--)
        LCDMEMW[i]=0xFFFF;
    LCDMEMW[9]=0xFFFF;

    for(int i=5;i;i--)
        LCDMEMW[i]=0;
    LCDMEMW[9]=0;
}

void TestLCD(){

    LCDMEMW[posw1] = letter['H'-'A'];
    LCDMEMW[posw2] = letter['A'-'A'];
    LCDMEMW[posw3] = letter['L'-'A'];
    LCDMEMW[posw4] = letter['L'-'A'];
    LCDMEMW[posw5] = letter['O'-'A'];
    LCDMEMW[posw6] = letter['W'-'A'];
    LCDMEMW[posw1] ^= ~(1<<8);
    LCDMEMW[posw1] ^= ~(1<<10);
    LCDMEMW[posw2] ^= ~(1<<8);
    LCDMEMW[posw2] ^= ~(1<<10);
    LCDMEMW[posw3] ^= ~(1<<8);
    LCDMEMW[posw3] ^= ~(1<<10);
    LCDMEMW[posw4] ^= ~(1<<8);
    LCDMEMW[posw4] ^= ~(1<<10);
    LCDMEMW[posw5] ^= ~(1<<8);
    LCDMEMW[posw5] ^= ~(1<<10);
    LCDMEMW[posw6] ^= ~(1<<8);
    LCDMEMW[posw6] ^= ~(1<<10);

    //9 = pos6

}
