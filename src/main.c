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
#include "settings.h"

unsigned long millis;

void mode_static() {
	uint8_t hue = analog_sample1();
	int8_t brightness = analog_sample2();
	setHSV(hue, 255, brightness);
}

void mode_cycle() {
	static uint8_t hue = 0;
	static unsigned long lastMillis = 0;

	uint8_t brightness = analog_sample1();
	uint8_t saturation = analog_sample2();

	if (millis>lastMillis+settings.delay) {
		setHSV(hue++, saturation, brightness);
		lastMillis = millis;
	}
}

enum Modes {
	MODE_STATIC,
	MODE_CYCLE,
	MODE_LAST
};

typedef void (*ModeHandler)(void);
ModeHandler modes[] = {
	mode_static,
	mode_cycle
};

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

	settings_read();

	DDRB |= _BV(DDB1) | _BV(DDB2) | _BV(DDB3); //set leds to output
	led_init();
	analog_init();

	static uint8_t oldBtnState = 0;

	rgb_t correction = {0xFF,0xFF,0xFF};
	rgb_t temperature = {0xFF,0xFF,0xFF};
	setAdjustment(255, &correction, &temperature);

	term_prompt();
	while(1) {
			term_read();

			uint8_t btnState = PINB & _BV(PB0);
			if (btnState != oldBtnState) {
				if (btnState) {
					settings.mode++;
					if (settings.mode == MODE_LAST) {
						settings.mode = 0;
					}
				}
				oldBtnState = btnState;
			}

			millis = timer_millis();
			modes[settings.mode]();
	}
}
