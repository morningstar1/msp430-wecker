#include <msp430.h>
#include "lib_sound.h"
#include "dcf77.h"
#include "runmode.h"
#include "lcd.h"
#include "menu.h"


int main(void) {
    WDTCTL = WDTPW + WDTHOLD;

    //BCSCTL1 = CALBC1_16MHZ | XT2OFF | DIVA_0;       // SMCLK=~16MHz
    //DCOCTL =  CALDCO_16MHZ;
    //BCSCTL3 = XCAP_3 | LFXT1S_0 | XT2S_0;

    // Configure XT1 oscillator
    P4SEL0 |= BIT1 | BIT2;                              // P4.2~P4.1: crystal pins
    do  {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);                  // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    }
    while (SFRIFG1 & OFIFG);                           // Test oscillator fault flag

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode

    CSCTL4 = SELMS__DCOCLKDIV + SELA__XT1CLK;           // MCLK=SMCLK=DCO; ACLK=XT1



    P1DIR |=  BIT0;        // P1.0 and P1.6 pins output the rest are input

    P1REN |=  BIT2;                     // Enable internal pull-up/down resistors
    P1OUT  =  BIT2;                     // Select pull-up mode for P1.3
    P1IE  |=  BIT2;                      // P1.3 interrupt enabled
    P1IES |=  BIT2;                     // P1.3 Hi/lo edge
    P1IFG &= ~BIT2;                    // P1.3 IFG cleared

    P2DIR |=  BIT7;
    P2OUT |=  BIT7;  //Power off DCF77
    P2REN |=  BIT6;                     // Enable internal pull-up/down resistors
    P2OUT  =  BIT6;                     // Select pull-up mode for P1.3
    P2IE  |=  BIT6;                      // P1.3 interrupt enabled
    P2IES |=  BIT6;                     // P1.3 Hi/lo edge
    P2IFG &= ~BIT6;                    // P1.3 IFG cleared

    initLCD();

    initCapturePort();

    init_sound();

    _EINT();

    while (1) {
        if(minute_done){
            decodeDCF();
        }
        showMenu();
        if (playsound) {
            lpm_state = 0;
            _BIS_SR(LPM0_bits + GIE);  // LPM0 und globale Interrupts aktivieren
        }else if(dcfCaptureRunning){
            lpm_state = 3;
            _BIS_SR(LPM3_bits + GIE);  // LPM3 und globale Interrupts aktivieren
        } else {
            lpm_state = 4;
            _BIS_SR(LPM4_bits + GIE);  // LPM3 und globale Interrupts aktivieren
        }
    }
    return 0;
}

__attribute__((interrupt(PORT1_VECTOR)))
void Port_1(void) {
    P1IFG &= ~BIT2;
    button1pressed = 1;
    EXIT_ISR;
}

__attribute__((interrupt(PORT2_VECTOR)))
void Port_2(void) {
    P2IFG &= ~BIT6;
    button2pressed = 1;
    EXIT_ISR;
}
