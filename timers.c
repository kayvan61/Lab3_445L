
#include "timers.h"

void DisableInterrupts(void); // Disable interrupts

void (*timer0A_Func)(void);
void (*timer1A_Func)(void);
void (*timer2A_Func)(void);
void (*timer3A_Func)(void);

void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout
  (*timer0A_Func)();                // execute user task
}

void Timer1A_Handler(void){
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout
  (*timer1A_Func)();                // execute user task
}

void Timer2A_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout
  (*timer2A_Func)();                // execute user task
}

void Timer3A_Handler(void){
  TIMER3_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout
  (*timer3A_Func)();                // execute user task
}

// at a 100 Hz frequency.  It is similar to FreqMeasure.c.
void Timer0A_Init1HzInt(void (*handler)(void)){
  volatile uint32_t delay;
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x01;      // activate timer0
  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
  TIMER0_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER0_CFG_R = 0;                // configure for 32-bit timer mode
  // **** timer0A initialization ****
                                   // configure for periodic mode
  TIMER0_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER0_TAILR_R = 79999999;         // start value for 100 Hz interrupts
  TIMER0_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// clear timer0A timeout flag
  TIMER0_CTL_R |= TIMER_CTL_TAEN;  // enable timer0A 32-b, periodic, interrupts
  // **** interrupt initialization ****
                                   // Timer0A=priority 2
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x00000000; // top 3 bits
  NVIC_EN0_R |= 1<<19;              // enable interrupt 19 in NVIC
	timer0A_Func = handler;
}

// at a 10k Hz frequency.  It is similar to FreqMeasure.c.
void Timer2A_Init10kHzInt(void (*handler)(void)){
  volatile uint32_t delay;
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x04;      // activate timer2
  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
  TIMER2_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER2_CFG_R = 0;                // configure for 32-bit timer mode
  // **** timer0A initialization ****
                                   // configure for periodic mode
  TIMER2_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER2_TAILR_R = 8000;         // start value for 10kHz interrupts
  TIMER2_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// clear timer2A timeout flag
  TIMER2_CTL_R |= TIMER_CTL_TAEN;  // enable timer2A 32-b, periodic, interrupts
  // **** interrupt initialization ****
                                   // Timer2A=priority 1
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x20000000; // top 3 bits
  NVIC_EN0_R |= 1<<23;              // enable interrupt 23 in NVIC
	timer2A_Func = handler;
}

// at a 10k Hz frequency.  It is similar to FreqMeasure.c.
void Timer3A_Init10kHzInt(void (*handler)(void)){
  volatile uint32_t delay;
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x08;      // activate timer2
  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
  TIMER3_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER3_CFG_R = 0;                // configure for 32-bit timer mode
  // **** timer0A initialization ****
                                   // configure for periodic mode
  TIMER3_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER3_TAILR_R = 8000;         // start value for 10kHz interrupts
  TIMER3_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER3_ICR_R = TIMER_ICR_TATOCINT;// clear timer2A timeout flag
  TIMER3_CTL_R |= TIMER_CTL_TAEN;  // enable timer2A 32-b, periodic, interrupts
  // **** interrupt initialization ****
                                   // Timer2A=priority 1
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0x20000000; // top 3 bits
  NVIC_EN1_R |= 1<<(3);              // enable interrupt 23 in NVIC
	timer3A_Func = handler;
}


void Timer1_Init(void (*handler)(void), uint32_t reload){ 
  volatile uint32_t delay;
  DisableInterrupts();
  // **** general initialization ****
  SYSCTL_RCGCTIMER_R |= 0x02;      // activate timer2
  delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
  TIMER1_CTL_R &= ~TIMER_CTL_TAEN; // disable timer0A during setup
  TIMER1_CFG_R = 0;                // configure for 32-bit timer mode
  // **** timer0A initialization ****
                                   // configure for periodic mode
  TIMER1_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
  TIMER1_TAILR_R = reload;         // start value for 10kHz interrupts
  TIMER1_IMR_R |= TIMER_IMR_TATOIM;// enable timeout (rollover) interrupt
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// clear timer2A timeout flag
  TIMER1_CTL_R |= TIMER_CTL_TAEN;  // enable timer2A 32-b, periodic, interrupts
  // **** interrupt initialization ****
                                   // Timer2A=priority 1
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF0FFF)|0x0000E000; // top 3 bits
  NVIC_EN0_R |= 1<<(21);              // enable interrupt 23 in NVIC
	timer1A_Func = handler;
}
