
#include "timers.h"
#include <stdint.h>

static uint8_t minutes = 0;
static uint8_t hours 	 = 0;

void clockInit(void){
	Timer0A_Init1HzInt();
}
