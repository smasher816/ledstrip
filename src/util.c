#include <avr/wdt.h>
#include <stdlib.h>
#include "print.h"

void version() {
	println(__DATE__" "__TIME__);
}

int free_ram() {
	extern int __heap_start, *__brkval;
	int stack;
	int free = (int)&stack - (__brkval==0 ? (int)&__heap_start : (int)__brkval);
	return free;
}

void ram() {
	char s[5];
	itoa(free_ram(), s, 10);
	puts(s);
}

void reset() {
	wdt_enable(WDTO_1S);
	while (1);
}
