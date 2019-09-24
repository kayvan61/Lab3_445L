#include "buttonDriver.h"
#include "../inc/tm4c123gh6pm.h"
#include "timers.h"

static uint8_t INPUT_MASK = 0x0;

//button is on A
void buttonInit(uint8_t inMask, void (*h)(void)){
	INPUT_MASK = ~inMask;
	volatile int delay;
	
	//Turn on the port clock
	SYSCTL_RCGCGPIO_R |= 0x04;
	
	delay = SYSCTL_RCGCGPIO_R;
	
	//Set in IO mask to the user input
	GPIO_PORTC_DIR_R = INPUT_MASK;
	
	//Diable the alt function based on the mask
  GPIO_PORTC_AFSEL_R = ~INPUT_MASK;
	
	//Ensure inputs are pulled down and not up
	GPIO_PORTC_PDR_R = ~INPUT_MASK;
	GPIO_PORTC_PUR_R = INPUT_MASK;
	
	//Enable digital on the input pins (this is an input only port)
  GPIO_PORTC_DEN_R = ~INPUT_MASK;
	
	//Turn off analog for all
  GPIO_PORTC_AMSEL_R = 0;

	Timer1_Init(h, 8000000);
}

uint8_t pollButton(void){
	return ((~INPUT_MASK) & GPIO_PORTC_DATA_R) & 0x000000FF;
}
