

#ifndef STELLARINO_INTERRUPTS_H
#define STELLARINO_INTERRUPTS_H

#include <stellarino.h>


#define CHANGE 4
#define FALLING 3
#define RISING 2
//#define HIGH 0x1
//#define LOW  0x0


void detachInterrupt(uint8_t pin);
void attachInterrupt(uint8_t pin, void (*userFunc)(int), int mode);

void initInterrupts();

#endif
