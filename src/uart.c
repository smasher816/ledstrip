#include "uart.h"

FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

void uart_init(void) {
#include <util/setbaud.h>
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
#if USE_2X
	UCSR0A |= _BV(U2X0);
#else
	UCSR0A &= ~(_BV(U2X0));
#endif
	UCSR0C = UCSRC_VALUE;
	UCSR0B = UCSRB_VALUE | _BV(RXEN0) | _BV(TXEN0); //enable rx, tx
}

int uart_putchar(char c, FILE *stream __attribute__((unused))) {
	if (c == '\n') {
		uart_putchar('\r', stream);
	}
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

int uart_getchar(FILE *stream __attribute__((unused))) {
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}

