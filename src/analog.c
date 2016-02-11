#include <avr/io.h>

void analog_init() {
	ADMUX = 0;//ADMUX = _BV(REFS0); //use AVCC as reference
	ADMUX |= _BV(MUX0);
	ADMUX |= _BV(ADLAR); //left alight (keep 8 msb, discard 2 least significant bits)

	ADCSRB = 0; //free run

	ADCSRA = _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); //128 prescaler
	ADCSRA |= _BV(ADATE); //auto trigger (gree run)
	ADCSRA |= _BV(ADEN); //enable adc
	ADCSRA |= _BV(ADSC); //start adc
}

uint8_t analog_sample() {
	return ADCH;
}
