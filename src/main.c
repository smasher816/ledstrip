#include <avr/io.h>
#include <avr/wdt.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "term.h"

#define LED_PIN PORTB1

void version() {
	println(__DATE__" "__TIME__);
}

int free_ram() {
	extern int __heap_start, *__brkval;
	int stack;
    int free = (int)&stack - (__brkval==0 ? (int)&__heap_start : (int)__brkval);
	return free;
}

void ram() {
	char s[5];
	itoa(free_ram(), s, 10);
	println(s);
}

void reset() {
	wdt_enable(WDTO_250MS);
	while (1);
}

void toggle_led() {
	PORTB ^= _BV(LED_PIN); //toggle led
}

int main(void) {
	MCUSR = 0;
	wdt_disable();

	uart_init();
	stdout = &uart_output;
    stdin  = &uart_input;

	term_clear();
	println("Hello World!");
	ram();

	DDRB |= _BV(DDB1); //set PB1 to output

	term_prompt();
	while(1) {
			term_read();
	}
}
