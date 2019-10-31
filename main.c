#include <msp430.h>
#include "lib_sound.h"
#include "dcf77.h"
#include "runmode.h"

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
    while (SFRIFG1 & OFIFG);


    P1DIR |= BIT0 + BIT6;        // P1.0 and P1.6 pins output the rest are input
    P1REN |= BIT3;                     // Enable internal pull-up/down resistors
    P1OUT = BIT3;                     // Select pull-up mode for P1.3
    P1IE |= BIT3;                      // P1.3 interrupt enabled
    P1IES |= BIT3;                     // P1.3 Hi/lo edge
    P1IFG &= ~BIT3;                    // P1.3 IFG cleared

    initPort();

    init_sound();
    _EINT();
    char buffer[32];

    while (1) {
        if(minute_done){
            decodeDCF(buffer);
        }
        /*
        if(counterready){
            uint8_t cr = counterready;
            counterready = 0;
            sprintf(buffer, "Counter %d %5u, bitcount %2d\r\n", cr, counter_diff, bitcount);
            UART_TX(buffer);
        }
        */
        //snprintf(buffer,32, "ps %d\r\n", playsound);
        //UART_TX(buffer);

        if (playsound) {
            //P1OUT |= BIT6;                      // Toggle P1.6
            //start_sound();
            lpm_state = 0;
            _BIS_SR(LPM0_bits + GIE);  // LPM0 und globale Interrupts aktivieren
        } else {
            //P1OUT &= ~BIT6;                      // Toggle P1.6
            //stop_sound();
            lpm_state = 3;
            _BIS_SR(LPM3_bits + GIE);  // LPM3 und globale Interrupts aktivieren
        }
    }
    return 0;
}

__attribute__((interrupt(PORT1_VECTOR)))
void Port_1(void) {
    P1IFG &= ~BIT3;                     // P1.3 IFG cleared
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
