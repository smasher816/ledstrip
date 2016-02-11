#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t adc0;
volatile uint8_t adc1;

ISR(ADC_vect) {
	if (bit_is_clear(ADMUX, MUX0)) {
		adc0 = ADCH;
	} else {
		adc1 = ADCH;
	}
	ADMUX ^= _BV(MUX0);
}

void analog_init() {
	ADCSRB = 0; //free run
	ADMUX = _BV(ADLAR); //keep high 8 msb
	ADCSRA = _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); //128 prescaler
	ADCSRA |= _BV(ADATE); //auto trigger (free run)
	ADCSRA |= _BV(ADIE); //enable adc interrupt
	ADCSRA |= _BV(ADEN); //enable adc
	ADCSRA |= _BV(ADSC); //start adc
}

uint8_t analog_sample1() {
	return adc0;
}

uint8_t analog_sample2() {
	return adc1;
}
