#include <msp430g2553.h>
#include "lib_sound.h"

uint8_t playsound = 0;
uint8_t lpm_state = 0;

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;
    BCSCTL1 = CALBC1_1MHZ | XT2OFF | DIVA_0;       // SMCLK=~16MHz
    DCOCTL = CALDCO_1MHZ;

    BCSCTL3 = XCAP_3 | LFXT1S_0 | XT2S_0;

    do {
        IFG1 &= ~OFIFG;   // IFG1=Interrupt Flag Register 1
        __delay_cycles(1000);

    } while (IFG1 & OFIFG);

    P1DIR |= BIT0 + BIT6;        // P1.0 and P1.6 pins output the rest are input
    P1REN |= BIT3;                     // Enable internal pull-up/down resistors
    P1OUT = BIT3;                     // Select pull-up mode for P1.3
    P1IE |= BIT3;                      // P1.3 interrupt enabled
    P1IES |= BIT3;                     // P1.3 Hi/lo edge
    P1IFG &= ~BIT3;                    // P1.3 IFG cleared

    init_sound();
    _EINT();

    while (1) {
        if (playsound) {
            P1OUT |= BIT6;                      // Toggle P1.6
            start_sound();
            lpm_state = 0;
            _BIS_SR(LPM0_bits + GIE);  // LPM0 und globale Interrupts aktivieren
        } else {
            P1OUT &= ~BIT6;                      // Toggle P1.6
            stop_sound();
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
