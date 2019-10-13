/*
 * melody.h
 *
 *  Created on: 13.10.2019
 *      Author: sebastian
 */

#ifndef MELODY_H_
#define MELODY_H_
#include <stdint.h>
extern const uint16_t imperialMarch[];
extern const uint16_t imperialMarchSize;
// Precalulated notes: CPUCLK/(note_freq*TimerDiv)
// 16MHz/(note_freq*8)
#define C8  477
#define B7  506
#define As7 536
#define A7  568
#define Gs7 601
#define G7  637
#define Fs7 675
#define F7  715
#define E7  758
#define Ds7 803
#define D7  851
#define Cs7 901
#define C7  955
#define B6  1012
#define As6 1072
#define A6  1136
#define Gs6 1203
#define G6  1275
#define Fs6 1351
#define F6  1431
#define E6  1516
#define Ds6 1607
#define D6  1702
#define Cs6 1803
#define C6  1911
#define B5  2024
#define As5 2145
#define A5  2272
#define Gs5 2407
#define G5  2551
#define Fs5 2702
#define F5  2863
#define E5  3033
#define Ds5 3214
#define D5  3405
#define Cs5 3607
#define C5  3822
#define B4  4049
#define As4 4290
#define A4  4545
#define Gs4 4815
#define G4  5102
#define Fs4 5405
#define F4  5726
#define E4  6067
#define Ds4 6428
#define D4  6810
#define Cs4 7215
#define C4  7644
#define B3  8099
#define As3 8580
#define A3  9090
#define Gs3 9631
#define G3  10204
#define Fs3 10810
#define F3  11453
#define E3  12134
#define Ds3 12856
#define D3  13621
#define Cs3 14430
#define C3  15289
#define B2  16198
#define As2 17161
#define A2  18181
#define Gs2 19262
#define G2  20408
#define Fs2 21621
#define F2  22907
#define E2  24269
#define Ds2 25712
#define D2  27241
#define Cs2 28861
#define C2  30578
#define B1  32396
#define As1 34322
#define A1  36363
#define Gs1 38525
#define G1  40816
#define Fs1 43243
#define F1  45815
#define E1  48539
#define Ds1 51425
#define D1  54483
#define Cs1 57723
#define C1  61156
#define B0  64792

#endif /* MELODY_H_ */
