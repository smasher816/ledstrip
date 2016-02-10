#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 500

int main (void) {
	/* set pin 1 of PORTB to output */
	DDRB |= _BV(DDB1);

	while(1) {
		/* set pin 5 high to turn led on */
		PORTB |= _BV(PORTB1);
		_delay_ms(BLINK_DELAY_MS);

		/* set pin 5 low to turn led off */
		PORTB &= ~_BV(PORTB1);
		_delay_ms(BLINK_DELAY_MS);
	}
}
