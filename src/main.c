#include <avr/io.h>
#include <avr/wdt.h>
#include <stdio.h>
#include <stdlib.h>
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
#include "ssd1306.h"

#define MAX(x) ((1<<(8*sizeof(x)))-1)
#define SCALE(x,min,max) (min+x*(max-min)/MAX(x))

char entries[][16] = {
  "Hello World",
  "OLED's are cool",
  "Foo",
  "Bar",
  "Something",
  "Rotary Encoder",
  "255,255,255",
  "#FFAABB"
};
int currentIndex = 2;

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

	/*uart_init();
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
	led_init();*/

	oled_init();
	oled_clear();

	char s[10] = {' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'};
	int frames = 0;
	unsigned long lastUpdate = 0;
	static uint8_t oldBtnState = 0;
	static uint8_t counter = 100;

	//setAdjustment(settings.brightness, &settings.correction, &settings.temperature);

	//term_prompt();
	while(1) {
			//term_read();

			////
			frames++;
			unsigned long millis = timer_millis();
			if (millis-lastUpdate > 1000) {
				itoa(frames, s, 10);
				frames = 0;
				lastUpdate = millis;
			}

			oled_move(0,0);
			oled_puts(s);

			for (int i=0; i<7; i++) {
				oled_move(i+1,0);
				if (i == currentIndex) {
					oled_putc('>');
				} else {
					oled_putc(' ');
				}
				oled_puts(entries[i]);
			}
			////

			/*uint8_t btnState = PINB & _BV(PB0);
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
			}*/
	}
}
