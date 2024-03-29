#include <msp430.h>
#include "lib_sound.h"
#include "melody.h"
#include "runmode.h"

uint8_t snd_notes_pos;

void setNoteForTimer() {
    // set note
    uint16_t note = imperialMarch[snd_notes_pos++];
    uint16_t duration = (imperialMarch[snd_notes_pos++]);
    TA0CCR0 = note;
    TA0CCR1 = TA0CCR0 >> DUTY_CYCLE;

    // set duration (32768*snd_ms_tick*notes_duration[snd_notes_pos++])/1000-DEAD_MS;
    TA1CCR0 += (duration - DEAD_MS) << 5;
}

void init_sound(void) {

    snd_notes_pos = 0;    // notes position (current pattern)
    playsound = 0;
    // TimerA0 for NOTE FREQUENCY
    TA0CTL |= TASSEL_2 + MC_0 + ID_0; // SMCLK=16MHz, Timer Div: 8, timer in stop mode
    TA0CCTL1 |= OUTMOD_7;                 // reset/set (PWM)

    // TimerA1 for NOTE DURATION (BMP)
    TA1CTL |= TASSEL_1 + MC_2 + ID_0;         // 32kHz crystal, no divider
}

void toogle_sound(void){
    if(playsound){
        stop_sound();
    }else{
        start_sound();
    }
}

void start_sound(void) {
    playsound = 1;
    setNoteForTimer();
    TA0CTL |= MC_1;
    TA1CCTL0 |= CCIE;
}

void stop_sound(void) {
    playsound = 0;
    TA0CTL &= ~MC_1;
    TA1CCTL0 &= ~CCIE;
    snd_notes_pos = 0;
    setNoteForTimer();
}

__attribute__((interrupt(TIMER1_A0_VECTOR)))
void update_note(void) {
    static uint8_t snd_cut_off = 0;      // var for note dead time

    if (!snd_cut_off)            // DEAD TIME
    {
        TA0CCR0 = 0;   // stop note frequency timer (a ZERO will stop the timer)
        TA1CCR0 += (DEAD_MS << 5);
        snd_cut_off = 1; // will be used after the dead time is over (load next note)
    } else                        // NOTE TIME
    {
        setNoteForTimer();
        if (snd_notes_pos >= imperialMarchSize / sizeof(uint16_t)) {
            snd_notes_pos = 0;    // set note position to first note in pattern
        }
        snd_cut_off = 0;          // for dead time, see if(!snd_cut_off)
    }
}
