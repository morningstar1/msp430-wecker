#include <msp430.h>
#include "lib_sound.h"
#include "dcf77.h"
#include "runmode.h"
#include "lcd.h"
#include "menu.h"

void InitPorts(){
    P1DIR =  BIT0 | BIT7;

    P1SEL0 = BIT7;
    P1OUT =  BIT7 | BIT2;
    P1REN =  BIT2;                     // Enable internal pull-up/down resistors
    P1IE  =  BIT2;                      // P1.3 interrupt enabled
    P1IES =  BIT2;                     // P1.3 Hi/lo edge
    P1IFG = 0;

    P2DIR  =  BIT7;
    P2REN  =  BIT6;                     // Enable internal pull-up/down resistors
    P2OUT  =  BIT6 | BIT7;
    P2IE   =  BIT6;                      // P1.3 interrupt enabled
    P2IES  =  BIT6;                     // P1.3 Hi/lo edge
    P2IFG  =  0;                    // P1.3 IFG cleared

    P4SEL0 = BIT1 | BIT2;                              // P4.2~P4.1: crystal pins

    P8DIR  = 0;
    P8SEL0 = BIT3;

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode

}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;

    //BCSCTL1 = CALBC1_16MHZ | XT2OFF | DIVA_0;       // SMCLK=~16MHz
    //DCOCTL =  CALDCO_16MHZ;
    //BCSCTL3 = XCAP_3 | LFXT1S_0 | XT2S_0;
    InitPorts();
    // Configure XT1 oscillator
    do  {
        CSCTL7 &= ~(XT1OFFG | DCOFFG);                  // Clear XT1 and DCO fault flag
        SFRIFG1 &= ~OFIFG;
    }
    while (SFRIFG1 & OFIFG);                           // Test oscillator fault flag

    CSCTL4 = SELMS__DCOCLKDIV + SELA__XT1CLK;           // MCLK=SMCLK=DCO; ACLK=XT1

    if (SYSRSTIV == SYSRSTIV_LPM5WU) {
        // If LPM3.5 wakeup
    }else{
        // Configure RTC
        RTCMOD = (32 * 60);                   // Set RTC modulo to 32768 to trigger interrupt each second
        RTCCTL = RTCSS__XT1CLK | RTCPS__1024 | RTCSR | RTCIE;     // Initialize RTC to use XT1 and enable RTC interrupt
        initLCD();
        setCurrentMenu(0);
    }
    initCapturePort();
    init_sound();

    _EINT();

    while (1) {
        if(minute_done){
            if(decodeDCF()){
                //reset RTC counter
                RTCCTL |= RTCSR;
            }
        }
        showMenu();
        if (playsound) {
            lpm_state = 0;
            _BIS_SR(LPM0_bits + GIE);  // LPM0 und globale Interrupts aktivieren
        }else if(dcfCaptureRunning){
            lpm_state = 3;
            _BIS_SR(LPM3_bits + GIE);  // LPM3 und globale Interrupts aktivieren
        } else {
            lpm_state = 35;
            PMMCTL0_H = PMMPW_H;                    // Open PMM Registers for write
            PMMCTL0_L |= PMMREGOFF_L;               // and set PMMREGOFF
            _BIS_SR(LPM3_bits + GIE);  // LPM3 und globale Interrupts aktivieren
            __no_operation();
        }
    }
    return 0;
}

__attribute__ ((interrupt(RTC_VECTOR)))
void RTC_ISR (void)
{
    switch(__even_in_range(RTCIV, RTCIV_RTCIF))
    {
        case RTCIV_NONE:
            break;
        case RTCIV_RTCIF:
            incrementTime();
            EXIT_ISR;
            break;
        default:
            break;
    }
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
