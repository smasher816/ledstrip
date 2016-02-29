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
#include "msgeq7.h"
#include "mode.h"

#define MAX(x) ((1<<(8*sizeof(x)))-1)
#define SCALE(x,min,max) (min+x*(max-min)/MAX(x))

unsigned long millis;

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

	analog_init();
	analog_start();
	msgeq7_init();
	led_init();

	static uint8_t oldBtnState = 0;

	setAdjustment(settings.brightness, &settings.correction, &settings.temperature);

	term_prompt();
	while(1) {
			term_read();

			uint8_t btnState = PINB & _BV(PB0);
			if (btnState != oldBtnState) {
				if (btnState) {
					settings.preset++;
					if (settings.preset == settings.preset_count) {
						settings.preset = 0;
					}
					settings_preset(settings.preset);
				}
				oldBtnState = btnState;
			}

			*preset.input1.var = (uint8_t)SCALE(analog_sample1(), preset.input1.min, preset.input1.max);
			*preset.input2.var = (uint8_t)SCALE(analog_sample2(), preset.input2.min, preset.input2.max);

			millis = timer_millis();
			modes[preset.mode]();
	}
}
