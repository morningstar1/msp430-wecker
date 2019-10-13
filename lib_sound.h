/*
 * lib_sound.h
 *
 *  Created on: 13.10.2019
 *      Author: sebastian
 */

#ifndef LIB_SOUND_H_
#define LIB_SOUND_H_
#include <stdint.h>

#define SPEAKER         BIT2    // P1.2
#define LOOP            1       // play in loop
#define ONCE            0       // play once
#define PATTERN_SIZE    16      // size of pattern
#define DEAD_MS         20      // dead time of note (cut off)
#define DUTY_CYCLE      1       // 2^DUTY_CYLCE ; 2^1=2 (50%); 2^2=4 (25%); 2^3=8 (12.5%)
#define TICKS_PER_BEAT  4       // 4/4

void init_sound(void);
void start_sound(void);
void stop_sound(void);

#endif /* LIB_SOUND_H_ */
