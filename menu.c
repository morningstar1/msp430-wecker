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

uint8_t button1pressed = 0;
uint8_t button2pressed = 0;

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
__attribute__ ((persistent)) uint8_t currentMenu = 0;

void setCurrentMenu(uint8_t menu){
    SYSCFG0 &= ~PFWP;                   // Program FRAM write enable
    currentMenu = menu;
    SYSCFG0 |= PFWP;                    // Program FRAM write protected (not writable)

}

void showMenu(){
    if(button1pressed){
        setCurrentMenu((currentMenu+1) % 4);
    }

    menues[currentMenu].draw();
    button1pressed = 0;
    button2pressed = 0;
}

