/*
 * menu.c
 *
 *  Created on: 02.11.2019
 *      Author: sebastian
 */


#include <menu.h>
#include "lcd.h"
#include "dcf77.h"
#include "runmode.h"
#include "lib_sound.h"

uint8_t button1pressed;
uint8_t button2pressed;

struct Menu{
    void (*draw)();
};

void ShowDCF(){

    if(button2pressed){
        toggleCapture();
    }
    showDCFCounter();
}

void showMusic(){
    if(button2pressed){
        toogle_sound();
    }
    LCDMEMW[posw1] = letter['M'-'A'];
    LCDMEMW[posw2] = letter['U'-'A'];
    LCDMEMW[posw3] = letter['S'-'A'];
    LCDMEMW[posw4] = letter['I'-'A'];
    LCDMEMW[posw5] = letter['C'-'A'] | 0x0500;
    LCDMEMW[posw6] = digit[playsound];
}

struct Menu menues[] = {{&showTime}, {&showDate}, {&ShowDCF}, {&showMusic}};
uint8_t currentMenu = 0;

void showMenu(){
    if(button1pressed){
        currentMenu = (currentMenu+1) % 4;
    }

    menues[currentMenu].draw();
    button1pressed = 0;
    button2pressed = 0;
}

