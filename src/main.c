#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "uart.h"
#include "term.h"
#include "led.h"

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
	puts(s);
}

void reset() {
	wdt_enable(WDTO_1S);
	while (1);
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

	uint8_t hue = 0;
	DDRB |= _BV(DDB1) | _BV(DDB2) | _BV(DDB3); //set leds to output
	led_init();

	term_prompt();
	while(1) {
			term_read();
			setHue(hue++);
			_delay_ms(10);
	}
}
