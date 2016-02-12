#include <avr/io.h>
#include <avr/wdt.h>
#include <stdio.h>
#include "uart.h"
#include "print.h"
#include "term.h"
#include "util.h"
#include "led.h"
#include "time.h"
#include "analog.h"

int delay = 10;

int main(void) {
	MCUSR = 0;
	wdt_disable();
	timer_init();

	uart_init();
	stdout = &uart_output;
    stdin  = &uart_input;

	term_clear();
	println("Hello World!");
	ram();

	uint8_t hue = 0;
	DDRB |= _BV(DDB1) | _BV(DDB2) | _BV(DDB3); //set leds to output
	led_init();

	analog_init();

	unsigned long oldMillis = 0;
	unsigned long newMillis;
	term_prompt();
	while(1) {
			term_read();
			newMillis = millis();
			uint8_t brightness = analog_sample1();
			uint8_t saturation = analog_sample2();
			if (newMillis>oldMillis+delay) {
				setHSV(hue++, saturation, brightness);
				oldMillis = newMillis;
			}
	}
}
