#include <avr/wdt.h>
#include <stdlib.h>
#include "print.h"
#include "var.h"

#define ARRAYLEN(a) (sizeof(a)/sizeof(a[0]))


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

void list_vars() {
	for(unsigned int i=0; i<ARRAYLEN(vars); i++) {
		puts_P(vars[i].name);
	}
}

void set(int argc, char *argv[]) {
	if (argc!=2) {
		ERR("set <var> <val>");
		list_vars();
		return;
	}
	for(unsigned int i=0; i<ARRAYLEN(vars); i++) {
		if (strcmp_P(argv[0], vars[i].name)==0) {
			switch (vars[i].type) {
				case Integer:
					*((int*)vars[i].value) = atoi(argv[1]);
					break;
				case Byte:
					*((uint8_t*)vars[i].value) = atoi(argv[1]);
					break;
				default:
					ERR("Unknown type");
			}
			return;
		}
	}
	ERR("Unknown variable");
}

void get(int argc, char *argv[]) {
	if (argc!=1) {
		ERR("get <var>");
		list_vars();
		return;
	}
	for(unsigned int i=0; i<ARRAYLEN(vars); i++) {
		if (strcmp_P(argv[0], vars[i].name)==0) {
			switch (vars[i].type) {
				case Integer: {
					char s[6];
					itoa(*((int*)vars[i].value), s, 10);
					puts(s);
					break;
				}
				case Byte: {
					char s[4];
					itoa(*((uint8_t*)vars[i].value), s, 10);
					puts(s);
					break;
				}
				default:
					ERR("Unknown type");
			}
			return;
		}
	}
}
