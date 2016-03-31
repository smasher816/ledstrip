#include <avr/pgmspace.h>
#include <stdlib.h>
#include "print.h"
#include "settings.h"
#include "vars.h"


/*const char str_hue[]   PROGMEM = "hue";
const char str_delay[] PROGMEM = "delay";
const char str_freq[]  PROGMEM = "freq";
const char str_fps[]   PROGMEM = "fps";
const char str_sens[]  PROGMEM = "sens";
const char str_mmin[]  PROGMEM = "bright";

{str_hue,   Byte,    &settings.hue},
{str_delay, Integer, &settings.delay},
{str_freq,  Byte,    &settings.music_frequency},
{str_fps,   Byte,    &settings.music_fps},
{str_sens,  Byte,    &settings.music_sensitivity},
{str_mmin,  Byte,    &settings.music_min_brightness}

uint8_t var_count = sizeof(vars)/sizeof(Var);*/

void list_vars() {
	print(vars_settings.name); print(":\n");
	for(uint8_t i=0; i<vars_settings.count; i++) {
		puts_P(&settings+vars_settings.vars[i].offset, argv[1]);
	}
	for(uint8_t i=0; i<MODE_COUNT; i++) {
		uint8_t mode = preset.modes[i].mode;
		print(mode_vars[mode].name); print(":\n");
		for(uint8_t j=0; j<mode_vars[mode].count; j++) {
			puts_P(&preset.config+mode_vars[mode][j].offset, argv[1]);
		}
	}
}

void set(Var *var, char *val) {
	switch (var.type) {
		case Integer:
			*((int*)var.value) = atoi(val);
			break;
		case Byte:
			*((uint8_t*)var.value) = atoi(val);
			break;
		default:
			ERR("Unknown type");
	}
}

void get(Var *var, char *val) {
	switch (var.type) {
		case Integer: {
			char s[6];
			itoa(*((int*)var.value), s, 10);
			puts(s);
			break;
		}
		case Byte: {
			char s[4];
			itoa(*((uint8_t*)var.value), s, 10);
			puts(s);
			break;
		}
		default:
			ERR("Unknown type");
	}
}

void var_set(int argc, char *argv[]) {
	if (argc!=2) {
		ERR("set <var> <val>");
		list_vars();
		return;
	}
	for(uint8_t i=0; i<vars_count_settings; i++) {
		if (strcmp_P(argv[0], vars_settings[i].name)==0) {
			set(&settings+vars_settings[i].offset, argv[1]);
			return;
		}
	}
	for(uint8_t i=0; i<vars_count_presets[0]; i++) {
		if (strcmp_P(argv[0], vars_presets[0][i].name)==0) {
			set(&preset+vars_presets[0][i].offset, argv[1]);
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
	for(uint8_t i=0; i<vars_count_settings; i++) {
		if (strcmp_P(argv[0], vars_settings[i].name)==0) {
			set(&settings+vars_settings[i].offset, argv[1]);
			return;
		}
	}
	for(uint8_t i=0; i<vars_count_presets[0]; i++) {
		if (strcmp_P(argv[0], vars_presets[0][i].name)==0) {
			set(&preset+vars_presets[0][i].offset, argv[1]);
			return;
		}
	}
	ERR("Unknown variable");
}
