// ADCTestMain.c
// Runs on TM4C123
// This program periodically samples ADC channel 0 and stores the
// result to a global variable that can be accessed with the JTAG
// debugger and viewed with the variable watch feature.
// Daniel Valvano
// September 5, 2015

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

#include <stdint.h>
#include "../inc/UART.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/PLL.h"
#include "ST7735.h"
#include "buttonDriver.h"
#include "Clock.h"
#include "speaker.h"

#define PF4             (*((volatile uint32_t *)0x40025010))
#define PF2             (*((volatile uint32_t *)0x40025010))
#define PF1             (*((volatile uint32_t *)0x40025008))
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

void initPortFDebug(void);
void buttonLogic(void);
void DelayWait10ms(uint32_t n);

//port C is for buttons 
//port B is for audio

uint8_t isAInput = 1;
uint8_t isCInput = 0;
uint8_t min = 0;
uint8_t hour = 0;
int main(void){
  PLL_Init(Bus80MHz);                   // 80 MHz
  ST7735_InitR(INITR_REDTAB);
	clockInit();
	buttonInit(0x00F0, buttonLogic);
	speakerInit();
	initPortFDebug();
	UART_Init();
	
  EnableInterrupts();
	displayClock();
	
	while(1) {
		if(getStale()){
					displayClock();
		}
		if(!isAInput && !isCInput) {
			hour = 0;
			min = 0;
			clearSet();
			continue;
		}
		//displayClock();
		uint8_t buttonState = pollButton();
		DelayWait10ms(100);
		if(buttonState & 0x40) {
			min++;
		}
		if(buttonState & 0x20) {
			hour++;
		}
		
		if (min >= 60){
			hour += 1;
			min = 0;
		}
	
		if (hour >= 24) {
			hour = 0;
		}
		if(isAInput){
			displayAlarm(min, hour);
		}
		if(isCInput){
			updateClock(min, hour);
		}
	}
	
}

uint8_t prevBtnStat;

void buttonLogic(void) {
	PF1 ^= 0x2;
	uint8_t buttonState = pollButton();
	if(buttonState & 0x10 && !(prevBtnStat & 0x10)) {
		toggleAlarm();
		isAInput = !isAInput;
	}
	if(buttonState & 0x80 && !(prevBtnStat & 0x80)) {
		isCInput = !isCInput;
	}
	prevBtnStat = buttonState;
}

void DelayWait10ms(uint32_t n){uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec
    while(time){
	  	time--;
    }
    n--;
  }
}

void initPortFDebug(void) {
	volatile int delay;
	SYSCTL_RCGCGPIO_R |= 0x20;            // activate port F
	delay = SYSCTL_RCGCGPIO_R;
	GPIO_PORTF_DIR_R |= 0x06;             // make PF2, PF1 out (built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x06;          // disable alt funct on PF2, PF1
  GPIO_PORTF_DEN_R |= 0x06;             // enable digital I/O on PF2, PF1
                                        // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF00F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;               // disable analog functionality on PF
  PF2 = 0;                      // turn off LED
}
