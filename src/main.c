#include <avr/io.h>
#include <stdio.h>
#include "uart.h"
#include "term.h"

#define LED_PIN PORTB1

int main(void) {
	uart_init();
	stdout = &uart_output;
    stdin  = &uart_input;
	term_clear();
	puts("Hello World!\n");

	DDRB |= _BV(DDB1); //set PB1 to output

	term_prompt();
	while(1) {
			if (term_read())
				PORTB ^= _BV(LED_PIN); //toggle led
	}
}
