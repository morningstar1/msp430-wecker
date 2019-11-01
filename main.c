#include <msp430.h>
#include "lib_sound.h"
#include "dcf77.h"
#include "runmode.h"
#include "lcd.h"

uint8_t playsound = 0;

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

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
    PM5CTL0 &= ~LOCKLPM5;

    CSCTL4 = SELMS__DCOCLKDIV + SELA__XT1CLK;           // MCLK=SMCLK=DCO; ACLK=XT1



    P1DIR |= BIT0;        // P1.0 and P1.6 pins output the rest are input
    P1REN |= BIT2;                     // Enable internal pull-up/down resistors
    P1OUT = BIT2;                     // Select pull-up mode for P1.3
    P1IE |= BIT2;                      // P1.3 interrupt enabled
    P1IES |= BIT2;                     // P1.3 Hi/lo edge
    P1IFG &= ~BIT2;                    // P1.3 IFG cleared

    initLCD();

    initCapturePort();

    init_sound();
    _EINT();

    while (1) {
        _DINT();
        if(minute_done){
            decodeDCF();
        }
        showDCFCounter();
        _EINT();


        if (playsound) {
            //P1OUT |= BIT0;                      // Toggle P1.6
            //start_sound();
            lpm_state = 0;
            _BIS_SR(LPM0_bits + GIE);  // LPM0 und globale Interrupts aktivieren
        } else {
            //P1OUT &= ~BIT0;                      // Toggle P1.6
            //stop_sound();
            lpm_state = 3;
            _BIS_SR(LPM3_bits + GIE);  // LPM3 und globale Interrupts aktivieren
        }
    }
    return 0;
}

__attribute__((interrupt(PORT1_VECTOR)))
void Port_1(void) {
    P1IFG &= ~BIT2;
    playsound = !playsound;
    switch (lpm_state) {
    case 0:
        LPM0_EXIT;
        break;
    case 3:
        LPM3_EXIT;
        break;
    case 4:
        LPM4_EXIT;
        break;
    }
}
