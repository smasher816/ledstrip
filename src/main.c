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

#define ENC_A PD2
#define ENC_B PD4
void enc_init() {
	DDRD &= ~(_BV(ENC_A) | _BV(ENC_B)); //input
	PORTD |= _BV(ENC_A) | _BV(ENC_B); //pull-up
}

int8_t enc_read() {
	static int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
	static uint8_t AB = 3;

	uint8_t a = ((PIND & _BV(ENC_A))!=0);
	uint8_t b = ((PIND & _BV(ENC_B))!=0);

	AB = (AB<<2) | (a<<1) | b;
	int8_t delta = enc_states[AB & 0x0F];

	//only count valid transitions at a detent
	return (delta!=0 && a==1 && b==1) ? delta : 0;
}

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
	enc_init();
	msgeq7_init();
	led_init();

	static uint8_t oldBtnState = 0;
	static uint8_t counter = 100;

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

			uint8_t x = enc_read();
			if (x) {
				counter += x;
				puti(counter); putchar('\n');
			}

			*preset.input1.var = (uint8_t)SCALE(analog_sample1(), preset.input1.min, preset.input1.max);
			*preset.input2.var = (uint8_t)SCALE(analog_sample2(), preset.input2.min, preset.input2.max);

			millis = timer_millis();
			for (int i=0; i<MODE_COUNT; i++) {
				mode_handlers[preset.modes[i].mode](&preset.modes[i].config);
			}
	}
}
