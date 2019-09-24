#ifndef BTTNDVR
#define BTTNDVR

#include<stdint.h>

void buttonInit(uint8_t, void (*h)(void));

uint8_t pollButton(void);

#endif
