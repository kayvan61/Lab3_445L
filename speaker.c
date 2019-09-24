#include "speaker.h"
#include <stdint.h>
#include "timers.h"
#include "../inc/tm4c123gh6pm.h"
#include "Clock.h"

static int ALARM_SONG[] = {440, 494, 523, 587, 659, 698};
static int counter = 0;
static int index = 0;

void speakerWork(void){
	if(!isArmed()) {
		counter = 0;
		index = 0;
		return;
	}
	counter++;
	if(counter > 100){
		index++;
		if(index > 5) {
			index = 0;
		}
		counter = 0;
		setFreq(ALARM_SONG[index]);
	}
	GPIO_PORTB_DATA_R ^= 0x40;
	(*((volatile uint32_t *)0x40025010)) ^= 0x04;
}

void speakerInit(void) {
	Timer3A_Init10kHzInt(&speakerWork);
	
	volatile int delay;
	
	//Turn on the port clock
	SYSCTL_RCGCGPIO_R |= 0x02;
	
	delay = SYSCTL_RCGCGPIO_R;
	
	//Set in IO mask to the user input
	GPIO_PORTB_DIR_R |= 0x40;
	
	//Diable the alt function based on the mask
  GPIO_PORTB_AFSEL_R = 0;
	
	//Enable digital on the input pins (this is an input only port)
  GPIO_PORTB_DEN_R |= (0x40);
	
	//Turn off analog for all
  GPIO_PORTB_AMSEL_R = 0;
	GPIO_PORTB_ODR_R = 0;

	speakerStop();
	setFreq(440);
	speakerStart();
}
void setFreq(uint32_t freq) {
	SYSCTL_RCGCTIMER_R |= 0x08;
	TIMER3_TAILR_R = 80000000/freq;
	SYSCTL_RCGCTIMER_R &= ~0x08;
}

void speakerStart(void) {
	SYSCTL_RCGCTIMER_R |= 0x08;
}

void speakerStop(void) {
	SYSCTL_RCGCTIMER_R &= ~0x08;
}
