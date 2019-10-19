#include <msp430g2553.h>
#include <stdio.h>
#include "lib_sound.h"
#include "dcf77.h"
#include "runmode.h"

uint8_t playsound = 0;

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;
    BCSCTL1 = CALBC1_16MHZ | XT2OFF | DIVA_0;       // SMCLK=~16MHz
    DCOCTL =  CALDCO_16MHZ;

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

    P1SEL  |= BIT1 | BIT2;                // Select TX and RX functionality for P1.1 & P1.2
    P1SEL2 |= BIT1 | BIT2;              //
    UCA0CTL1 |= UCSSEL_2;             // Have USCI use System Master Clock: AKA core clk 1MHz
    //UCA0BR0 = 104;                    // 1MHz 9600, see user manual
    //UCA0BR1 = 0;                      //
    uint16_t br = 104;
    UCA0BR0 = br & 0xFF;                    // 1MHz 9600, see user manual
    UCA0BR1 = (br >> 8) & 0xFF;                      //

    UCA0MCTL = UCBRS_0 | UCBRF_3 | UCOS16;                // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;             // Start USCI state machine

    initPort();

    init_sound();
    UART_TX("Welcome, to Quake ]I[ Arena\r\n");
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

void UART_TX(char * tx_data) // Define a function which accepts a character pointer to an array
{
    /*
    unsigned int i=0;
    while(tx_data[i]) // Increment through array, look for null pointer (0) at end of string
    {
        while (!(IFG2 & UCA0TXIFG));                                         // Warten bis USART0 TX-Puffer frei (leer) ...

        //while ((UCA0STAT & UCBUSY)); // Wait if line TX/RX module is busy with data
        UCA0TXBUF = tx_data[i]; // Send out element i of tx_data array on UART bus
        i++; // Increment variable for array address
    }*/

    while (*tx_data != 0)                                                              // Solange nicht das Terminiert-Zeichen kommt ...
    {
      while (!(IFG2 & UCA0TXIFG));                                         // Warten bis USART0 TX-Puffer frei (leer) ...
      UCA0TXBUF = *tx_data++;                                                          // Zeichen dazufügen.-
    } // while
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
