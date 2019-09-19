
#ifndef timersh
#define timersh

#include "../inc/tm4c123gh6pm.h" 
#include <stdint.h>
// at a 100 Hz frequency.  It is similar to FreqMeasure.c.
void Timer0A_Init1HzInt(void);

void Timer1_Init(void);

void Timer2A_Init10kHzInt(void);

void Timer3A_Init10kHzInt(void);

#endif
