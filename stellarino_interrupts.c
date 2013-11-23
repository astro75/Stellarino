
// Ported from Energia

#include <stellarino_interrupts.h>

static void (*cbFuncs[6][8])(int);

void attachInterrupt(uint8_t pin, void (*userFunc)(int), int mode) {
	uint32_t lm4fMode;

	uint8_t bit = bit8[pin%8];
	uint32_t portBase = GPIO[pin/8];

	switch(mode) {
	case LOW:
		lm4fMode = GPIO_LOW_LEVEL;
		break;
	case CHANGE:
		lm4fMode = GPIO_BOTH_EDGES;
		break;
	case RISING:
		lm4fMode = GPIO_RISING_EDGE;
		break;
	case FALLING:
		lm4fMode = GPIO_FALLING_EDGE;
		break;
	default:
		return;
	}

	ROM_IntMasterDisable();
	ROM_GPIOPinIntClear(portBase, bit);
	ROM_GPIOIntTypeSet(portBase, bit, lm4fMode);
	ROM_GPIOPinIntEnable(portBase, bit);

	cbFuncs[pin/8][pin%8] = userFunc;

	ROM_IntMasterEnable();

}

void detachInterrupt(uint8_t pin) {

	uint8_t bit = bit8[pin%8];
	uint32_t portBase = GPIO[pin/8];

	ROM_GPIOPinIntDisable(portBase, bit);

	cbFuncs[pin/8][pin%8] = 0;
}

void GPIOIntHandler(uint8_t port) {
	uint32_t i;
	uint32_t isr = ROM_GPIOPinIntStatus(GPIO[port], true);

	ROM_GPIOPinIntClear(GPIO[port], isr);

	for (i=0; i<8; i++, isr>>=1) {
		if ((isr & 0x1) == 0)
			continue;
		if (cbFuncs[port][i])
			cbFuncs[port][i](port * 8 + i);
	}
}

void GPIOAIntHandler(void) { GPIOIntHandler(0); }
void GPIOBIntHandler(void) { GPIOIntHandler(1); }
void GPIOCIntHandler(void) { GPIOIntHandler(2); }
void GPIODIntHandler(void) { GPIOIntHandler(3); }
void GPIOEIntHandler(void) { GPIOIntHandler(4); }
void GPIOFIntHandler(void) { GPIOIntHandler(5); }

void initInterrupts() {
	GPIOPortIntRegister(GPIO_PORTA_BASE, GPIOAIntHandler);
	GPIOPortIntRegister(GPIO_PORTB_BASE, GPIOBIntHandler);
	GPIOPortIntRegister(GPIO_PORTC_BASE, GPIOCIntHandler);
	GPIOPortIntRegister(GPIO_PORTD_BASE, GPIODIntHandler);
	GPIOPortIntRegister(GPIO_PORTE_BASE, GPIOEIntHandler);
	GPIOPortIntRegister(GPIO_PORTF_BASE, GPIOFIntHandler);
}
