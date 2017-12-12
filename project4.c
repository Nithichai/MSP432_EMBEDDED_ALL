#include "msp.h"

char in;

void UART0_init(void);
void UART0_send(char c);

int main(void) {
	__disable_irq();
	UART0_init();
	NVIC_SetPriority(EUSCIA0_IRQn, 1);
	NVIC_EnableIRQ(EUSCIA0_IRQn);
	__enable_irq();
	while (1) {
	}
}

void UART0_init(void){
	EUSCI_A0->CTLW0 |= 1;
	EUSCI_A0->MCTLW = 0;
	EUSCI_A0->CTLW0 = 0x0081;
	EUSCI_A0->BRW = 26;
	P1->SEL0 |= 0x0C;
	P1->SEL1 &= ~0x0C;
	EUSCI_A0->CTLW0 &= ~1;
	EUSCI_A0->IE |= 1;
}

void EUSCIA0_IRQHandler(void){
	in = EUSCI_A0->RXBUF;
	UART0_send(in);
}

void UART0_send(char out) {
	while(!(EUSCI_A0->IFG & 0x02)){}
		EUSCI_A0->TXBUF = out;
}
