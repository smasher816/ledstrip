#include <avr/io.h>
#include <stdio.h>
#include "uart.h"

#define LED_PIN PORTB1

int main(void) {
	uart_init();
	stdout = &uart_output;
    stdin  = &uart_input;
	puts("Hello World!\n");

	DDRB |= _BV(DDB1); //set PB1 to output

	char c;
	while(1) {
			c = getchar();
			putchar(c);
			PORTB ^= _BV(LED_PIN); //toggle led
	}
}
