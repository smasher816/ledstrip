//https://www.sparkfun.com/datasheets/Components/General/MSGEQ7.pdf
#include <avr/io.h>
#include <util/delay.h>
#include "analog.h"
#include "msgeq7.h"

#include "main.h"
#include "print.h"

uint8_t f[8][2];

void msgeq7_init() {
	DDRB |= _BV(PIN_STROBE) | _BV(PIN_RESET);
}

void msgeq7_reset() {
	PORTB &= ~_BV(PIN_STROBE);

	PORTB |= _BV(PIN_RESET);
	_delay_us(0.1); //tr
	PORTB &= ~_BV(PIN_RESET);
	_delay_us(72); //trs
}

void msgeq7_read() {
	analog_stop();
	msgeq7_reset();

	for (uint8_t i=0; i<FREQ_COUNT; i++) {
		PORTB |= _BV(PIN_STROBE);
		_delay_us(18); //ts
		PORTB &= ~_BV(PIN_STROBE);
		_delay_us(36); //to

		f[i][CHANNEL_LEFT] = analog_read(PIN_LEFT);
		f[i][CHANNEL_RIGHT] = analog_read(PIN_RIGHT);
		_delay_us(2); //tss-ts-to-2*tadc
	}

	analog_start();
}
