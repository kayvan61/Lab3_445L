#ifndef SPEAKERH
#define SPEAKERH

#include <stdint.h>

void speakerInit(void);
void setFreq(uint32_t);
void speakerStart(void);
void speakerStop(void);

#endif 
