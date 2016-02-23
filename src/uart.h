#pragma once
#include <avr/io.h>
#include <stdio.h>

#define BAUD_TOL 3
#define UCSRB_VALUE 0 //no interrupts
#define UCSRC_VALUE (_BV(UCSZ01) | _BV(UCSZ00)) //8-bit, 1 stop bit, no parity

void uart_init(void);
int uart_putchar(char, FILE *);
int uart_getchar(FILE *stream);
static inline int uart_available() {
	return bit_is_set(UCSR0A, RXC0);
}

extern FILE uart_output;
extern FILE uart_input;
