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
char *title = "MAIN";
int currentIndex = 1;

unsigned long millis;

#define BTN PD3
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
#ifdef ENC_DETENT
	return (delta!=0 && a==1 && b==1) ? delta : 0;
#else
	return delta;
#endif
}

void showVar(char* title, uint8_t val, uint8_t min, uint8_t max) {
	oled_move(0,0);//oled_move(0, (128-strlen(title))/2);
	oled_puts2(title);

	oled_move(4, 0);
	oled_puti(val, 3);

	oled_move(7,0);
	oled_pbar((val-min)/(max/127), 128);
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
	msgeq7_init();
	led_init();*/
	enc_init();

	oled_init();
	oled_clear();

	char fps[10] = {0};
	//char s2[10] = {0};
	int frames = 0;
	unsigned long lastUpdate = 0;
	static uint8_t oldBtnState = 1;
	int inMenu = 0;
	uint8_t val=127;

	DDRD &= ~_BV(BTN); //input
	PORTD |= _BV(BTN); //pull-up

	//setAdjustment(settings.brightness, &settings.correction, &settings.temperature);

	//term_prompt();
	while(1) {
			//term_read();

			////
			frames++;
			unsigned long millis = timer_millis();
			if (millis-lastUpdate > 1000) {
				itoa(frames, fps, 10);
				frames = 0;
				lastUpdate = millis;
			}

			/*oled_move(0,0);
			oled_puts(title);
			oled_move(0, 12);
			oled_puts(fps);

			if (inMenu) {
				oled_move(1,0);
				oled_puts("...");
			} else {
				oled_move(0,8);
				oled_puts(s2);
				for (int i=1; i<7; i++) {
					oled_move(i,0);
					if (i == currentIndex) {
						oled_putc('>');
					} else {
						oled_putc(' ');
					}
					oled_puts(entries[i-1]);
				}
			}*/
			////

			int8_t x = enc_read();
			if (x) {
				/*currentIndex += x;
				if (currentIndex<1) currentIndex = 1;
				if (currentIndex>6) currentIndex = 6;
				itoa(currentIndex, s2, 10);*/
				val += x;
				oled_set_contrast(val);
				showVar("BRIGHTNESS", val, 0, 255);
			}

			uint8_t btnState = PIND & _BV(BTN);
			if (btnState != oldBtnState) {
				if (btnState) {
					/*settings.preset++;
					if (settings.preset == settings.preset_count) {
						settings.preset = 0;
					}
					settings_preset(settings.preset);*/
					inMenu = !inMenu;
					if (inMenu) {
						title = entries[currentIndex-1];
					} else {
						title = "MAIN";
					}
					oled_clear();
				}
				oldBtnState = btnState;
			}

			/*
			 *preset.input1.var = (uint8_t)SCALE(analog_sample1(), preset.input1.min, preset.input1.max);
			*preset.input2.var = (uint8_t)SCALE(analog_sample2(), preset.input2.min, preset.input2.max);

			millis = timer_millis();
			for (int i=0; i<MODE_COUNT; i++) {
				mode_handlers[preset.modes[i].mode](&preset.modes[i].config);
			}
			*/
	}
}
