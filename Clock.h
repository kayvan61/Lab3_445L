#ifndef CLOCKH
#define CLOCKH

#include<stdint.h>

void clockInit(void);
uint16_t getClockState(void);
void tickClock(void);
void toggleAlarm(void);
uint8_t isArmed(void);
void displayClock(void);
void enterAlarmTime(void);
void displayAlarm(uint8_t min, uint8_t hour);
uint8_t getStale(void);
void updateClock(uint8_t min, uint8_t hour);
void clearSet(void);
void snooze(void);

#endif
