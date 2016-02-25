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

#define FPS2MS(x) (1000/x)

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

void mode_music() {
	static unsigned long lastMillis = 0;
	if (millis>lastMillis+FPS2MS(settings.music_fps)) {
		msgeq7_read();

		uint8_t hue = analog_sample1();
		uint8_t min = analog_sample2();

		uint16_t vol =f[settings.music_frequency][CHANNEL_LEFT];
		if (vol <= settings.msgeq7_min)
			vol = 0;

		vol = min + vol*(settings.music_sensitivity/100.0);
		if (vol > 255)
			vol = 255;

		setHSV(hue, 255, vol);
		lastMillis = millis;
	}

}

typedef void (*ModeHandler)(void);
ModeHandler modes[] = {
	mode_static,
	mode_cycle,
	mode_music
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

	analog_init();
	analog_start();
	msgeq7_init();
	led_init();

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
