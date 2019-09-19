#include "buttonDriver.h"
#include "../inc/tm4c123gh6pm.h"

static uint8_t INPUT_MASK = 0x0;

//button is on A
void buttonInit(uint8_t inMask){
	INPUT_MASK = ~inMask;
	
	//Turn on the port clock
	SYSCTL_RCGCGPIO_R |= 0x01;
	
	//Set in IO mask to the user input
	GPIO_PORTA_DIR_R = INPUT_MASK;
	
	//Diable the alt function based on the mask
  GPIO_PORTA_AFSEL_R = ~INPUT_MASK;
	
	//Ensure inputs are pulled down and not up
	GPIO_PORTA_PDR_R = ~INPUT_MASK;
	GPIO_PORTA_PUR_R = INPUT_MASK;
	
	//Enable digital on the input pins (this is an input only port)
  GPIO_PORTA_DEN_R = ~INPUT_MASK;
	
	//Turn off analog for all
  GPIO_PORTA_AMSEL_R = 0;

}

uint8_t pollButtons(void){
	return ((~INPUT_MASK) & GPIO_PORTA_DATA_R) & 0x000000FF;
}
