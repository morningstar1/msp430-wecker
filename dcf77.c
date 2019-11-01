#include <msp430.h>
#include <stdint.h>

#include "dcf77.h"
#include "runmode.h"
#include "lcd.h"

struct bsData{
    uint16_t bitstream[4];
    uint16_t bitcount;
} doublebuffer[2];

uint8_t read_index = 1;
uint8_t write_index = 0;

uint8_t minute_done = 0;

void initCapturePort(){
    //Setup P2.1
    P8DIR  &= ~BIT3;
    P8SEL0 |= BIT3;
    for(int i=0;i<4;i++){
        doublebuffer[0].bitcount = 0;
        doublebuffer[0].bitstream[i] = 0;
    }

    //TA1CCTL1 |= CM_3 | CCIS_0 | SCS | CAP | CCIE;
    TA1CCTL2 |= CM_3 | CCIS_0 | SCS | CAP | CCIE;
}

void pushBit(uint8_t bit){
    uint16_t bc = doublebuffer[write_index].bitcount;
    uint16_t mask = 1 << (bc & 0xF);
    uint16_t offset = (bc >> 4) & 0x3;
    if(bit){
        doublebuffer[write_index].bitstream[offset] |= mask;
    }else{
        doublebuffer[write_index].bitstream[offset] &= ~mask;

    }
    doublebuffer[write_index].bitcount++;
}

__attribute__((interrupt(TIMER1_A1_VECTOR)))
void edge_detect(void) {

    if(TA1CCTL2 & CCIFG){
        TA1CCTL2 &= ~CCIFG;
    }else{
        return;
    }
    //P1OUT ^= BIT0;                      // Toggle P1.6

    uint16_t counter = TA1CCR2;
    uint16_t new_state = TA1CCTL2 & CCI;
    static uint16_t last_state = 0;
    static uint16_t last_counter = 0;
    if(new_state ^ last_state){
        //state change detected
        uint16_t counter_diff = counter - last_counter;

        if(new_state){
            //rising edge
            if(counter_diff > 52428){
                //detected last Second;
                minute_done++;
                write_index ^= 1;
                read_index ^= 1;
            }
        }else{
            //falling edge
            const uint16_t zero_lowerlimit = 2458;
            const uint16_t zero_upperlimit = 4096;
            const uint16_t one_lowerlimit = 5734;
            const uint16_t one_upperlimit = 7373;
            if(counter_diff > zero_lowerlimit && counter_diff < zero_upperlimit){
                pushBit(0);
            }else if(counter_diff > one_lowerlimit && counter_diff < one_upperlimit){
                pushBit(1);
            }
        }
        last_counter = counter;
    }
    last_state = new_state;
    if(1 || minute_done > 0){
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
}

uint8_t calcParity(uint16_t * bs, uint8_t startbit, uint8_t count){
    uint8_t paritycount = 0;
    for (uint8_t bitpos = startbit; bitpos < startbit + count; bitpos++) {
        uint16_t mask = 1 << (bitpos & 0xF);
        uint8_t offset = (bitpos >> 4) & 0x3;
        if (bs[offset] & mask) {
            paritycount++;
        }
    }
    return paritycount & 1;
}

uint8_t extract(uint16_t * bs, uint8_t startbit, uint8_t count){
    if(startbit >> 4 != (startbit + count - 1) >> 4){
        //multibyte
        uint8_t lowercount = 16 - (startbit & 0xF);
        uint16_t lower_stuff = extract(bs, startbit, lowercount);
        uint16_t higher_stuff = extract(bs, startbit + lowercount, count - lowercount);
        higher_stuff <<= lowercount;
        return (lower_stuff + higher_stuff);
    }else{
        //single byte
        uint16_t mask = ((1 << count) - 1) << (startbit & 0xF);
        uint8_t offset = (startbit >> 4) & 0x3;
        return (bs[offset] & mask) >> (startbit & 0xF);
    }
}

void decodeDCF(){
    minute_done--;
    //isr can go on;
    if(doublebuffer[read_index].bitcount  >= 59){
        //got right bit count
        uint16_t * bs = doublebuffer[read_index].bitstream;
        uint8_t minute_bcd_low   = extract(bs, 21, 4);
        uint8_t minute_bcd_high  = extract(bs, 25, 3);
        //uint8_t minute_parity    = extract(bs, 28, 1);
        //uint8_t minute_parity_calc = calcParity(bs, 21, 28-21);
        LCDMEM[pos3] = digit[minute_bcd_high];
        LCDMEM[pos4] = digit[minute_bcd_low];

        uint8_t hour_bcd_low     = extract(bs, 29, 4);
        uint8_t hour_bcd_high    = extract(bs, 33, 2);
        //uint8_t hour_parity      = extract(bs, 35, 1);
        //uint8_t hour_parity_calc = calcParity(bs, 29, 36-29);
        LCDMEM[pos1] = digit[hour_bcd_high];
        LCDMEM[pos2] = digit[hour_bcd_low];

        /*
        uint8_t day_bcd_low      = extract(bs, 36, 4);
        uint8_t day_bcd_high     = extract(bs, 40, 2);
        uint8_t weekday          = extract(bs, 42, 3);
        uint8_t month_bcd_low    = extract(bs, 45, 4);
        uint8_t month_bcd_high   = extract(bs, 49, 1);
        uint8_t year_bcd_low     = extract(bs, 50, 4);
        uint8_t year_bcd_high    = extract(bs, 54, 4);
        uint8_t date_parity      = extract(bs, 58, 1);
        uint8_t date_parity_calc = calcParity(bs, 36, 58-36);
         */
        LCDMEM[pos1+1] = 0x00;
    }else{
        LCDMEM[pos1+1] = 0xFF;
    }
    doublebuffer[read_index].bitcount = 0;
    doublebuffer[read_index].bitstream[0] = 0;
    doublebuffer[read_index].bitstream[1] = 0;
    doublebuffer[read_index].bitstream[2] = 0;
    doublebuffer[read_index].bitstream[3] = 0;
}

void showDCFCounter(){
    LCDMEM[pos5] = digit[(doublebuffer[write_index].bitcount / 10)%10];
    LCDMEM[pos6] = digit[doublebuffer[write_index].bitcount % 10];
}
