#pragma once

#define FREQ_COUNT 7
#define CHANNEL_LEFT 0
#define CHANNEL_RIGHT 1

#define F_BASS 0

#define PIN_STROBE PORTB4
#define PIN_RESET PORTB5
#define PIN_LEFT _BV(MUX1) //ADC2
#define PIN_RIGHT _BV(MUX1)|_BV(MUX0) //ADC3

extern uint8_t f[8][2];

void msgeq7_init();
void msgeq7_read();
