#include "msp.h"
#include "string.h"
#define SYSTICK_STCSR (*((volatile unsigned long *)0xE000E010))
#define SYSTICK_STRVR (*((volatile unsigned long *)0xE000E014))
#define SYSTICK_STCVR (*((volatile unsigned long *)0xE000E018))

void SysTick_Init(void);
void SysTick_wait(uint32_t delay);
void SysTick_wait1ms(uint32_t delay);
void Port_Init(void);
void UART0_send(char c);
void Blink_Blue(void);
void Blink_Green(void);

int main(void) {
	Port_Init();
	SysTick_Init();
	__disable_irq();
	NVIC_EnableIRQ(PORT5_IRQn);
	NVIC_EnableIRQ(PORT6_IRQn);
	NVIC_EnableIRQ(EUSCIA0_IRQn);
	__enable_irq();
	while (1) {}
}

void PORT5_IRQHandler(void) {
	if (P5->IFG & 0x01) {
		SysTick_wait1ms(10);
		if (!(P5IN & 0x01)) {
			Blink_Blue();
		}
		P5->IFG &= ~0x01;
	}
}

void PORT6_IRQHandler(void) {
	if (P6->IFG & 0x01) {
		SysTick_wait1ms(10);
		if (!(P6IN & 0x01)) {
			Blink_Green();
		}
		P6->IFG &= ~0x01;
	}
}

void Blink_Blue(void) {
  for (int i = 0; i < 3; i++) {
    P2OUT = 0x04;
    SysTick_wait1ms(500);
    P2OUT = 0x00;
    SysTick_wait1ms(500);
  }
}

void Blink_Green(void) {
  for (int i = 0; i < 3; i++) {
    P2OUT = 0x02;
    SysTick_wait1ms(500);
    P2OUT = 0x00;
    SysTick_wait1ms(500);
  }
}

void Port_Init(void) {
	P5SEL1 = 0x00;
	P5SEL0 = 0x00;
	P5DIR = 0x00;
	P5REN = 0x01;
	P5OUT = 0x01;
	P5IES = 0x01;
	P5IFG = 0;
	P5IE = 0x01;

  P6SEL1 = 0x00;
	P6SEL0 = 0x00;
	P6DIR = 0x00;
	P6REN = 0x01;
	P6OUT = 0x01;
	P6IES = 0x01;
	P6IFG = 0;
	P6IE = 0x01;

  P2SEL1 = 0x00;
	P2SEL0 = 0x00;
	P2DIR = 0x07;
	P2OUT = 0x00;
}

void SysTick_Init(void) {
	SYSTICK_STCSR = 0;
	SYSTICK_STRVR = 0x00FFFFFF;
	SYSTICK_STCVR = 0;
	SYSTICK_STCSR = 0x00000005;
}

void SysTick_wait(uint32_t delay) {
	SYSTICK_STRVR = delay - 1;
	SYSTICK_STCVR = 0;
	while ((SYSTICK_STCSR & 0x00010000) == 0) {
	}
}

void SysTick_wait1ms(uint32_t delay) {
	uint32_t i;
	for (i = 0; i < delay; i++) {
		SysTick_wait(3000);
	}
}
