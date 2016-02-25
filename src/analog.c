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
	ADMUX ^= _BV(MUX0); //switch between both pot inputs
}

void analog_init() {
	DDRC = 0; //all input
	ADCSRB = 0; //no trigger (free run)
	ADCSRA = _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); //128 prescaler (125kHz clock)
	ADCSRA |= _BV(ADEN); //enable adc
}

void analog_start() {
	ADMUX = _BV(ADLAR); //keep high 8 msb
	ADCSRA |= _BV(ADATE); //auto trigger (free run)
	ADCSRA |= _BV(ADIE); //enable adc interrupt
	ADCSRA |= _BV(ADSC); //start adc
}

void analog_stop() {
	ADCSRA &= ~_BV(ADATE); //stop auto trigger
	ADCSRA &= ~_BV(ADIE); //no interrupt
}

uint8_t analog_read(uint8_t pin) {
	ADMUX = _BV(ADLAR) | pin; //set input pin and left align (get high 8 bits)
	ADCSRA |= _BV(ADSC); //start adc
	loop_until_bit_is_clear(ADCSRA, ADSC); //wait for conversion to finish (~8uS)
	return ADCH;
}

uint8_t analog_sample1() {
	return adc0;
}

uint8_t analog_sample2() {
	return adc1;
}
