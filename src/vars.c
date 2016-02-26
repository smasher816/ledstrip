#include <avr/pgmspace.h>
#include <stdlib.h>
#include "print.h"
#include "settings.h"
#include "vars.h"

const char str_mode[]  PROGMEM = "preset";
const char str_hue[]   PROGMEM = "hue";
const char str_delay[] PROGMEM = "delay";
const char str_min[]   PROGMEM = "min";
const char str_freq[]  PROGMEM = "freq";
const char str_fps[]   PROGMEM = "fps";
const char str_sens[]  PROGMEM = "sens";
const char str_mmin[]  PROGMEM = "bright";

Var vars[] = {
	{str_mode,  Byte,    &settings.preset},
	{str_hue,   Byte,    &settings.hue},
	{str_delay, Integer, &settings.delay},
	{str_min,   Byte,    &settings.msgeq7_min},
	{str_freq,  Byte,    &settings.music_frequency},
	{str_fps,   Byte,    &settings.music_fps},
	{str_sens,  Byte,    &settings.music_sensitivity},
	{str_mmin,  Byte,    &settings.music_min_brightness}
};

uint8_t var_count = sizeof(vars)/sizeof(Var);


void list_vars() {
	for(uint8_t i=0; i<var_count; i++) {
		puts_P(vars[i].name);
	}
}

void var_set(int argc, char *argv[]) {
	if (argc!=2) {
		ERR("set <var> <val>");
		list_vars();
		return;
	}
	for(uint8_t i=0; i<var_count; i++) {
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

void var_get(int argc, char *argv[]) {
	if (argc!=1) {
		ERR("get <var>");
		list_vars();
		return;
	}
	for(uint8_t i=0; i<var_count; i++) {
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
	ERR("Unknown variable");
}
