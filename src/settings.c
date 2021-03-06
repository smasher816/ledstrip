#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "settings.h"

typedef struct {
	Settings settings;
	Preset preset;
} EEPROM_t;

/*EEMEM EEPROM_t eeprom = {
	SETTINGS_DEFAULTS, \
	PRESET_DEFAULTS, \
};*/


const char str_settings[]  PROGMEM = "settings";
const char str_preset[]  PROGMEM = "preset";
const char str_min[]   PROGMEM = "min";

VarGroup vars_settings = {
	str_settings,
	2,
	{
		{str_preset, Byte, &settings.preset},
		{str_min,    Byte, &settings.msgeq7_min}
	}
};


Settings settings;
Preset preset;

void settings_read() {
	eeprom_read_block(&settings, 0x00, sizeof(Settings));
	settings_preset(settings.preset);
}

void settings_preset(uint8_t i) {
	eeprom_read_block(&preset, (void*)(sizeof(Settings)+i*sizeof(Preset)), sizeof(Preset));
}

void settings_write() {
	eeprom_update_block(&settings, 0x00, sizeof(Settings));
	eeprom_update_block(&preset, (void*)sizeof(Settings), sizeof(Preset));
}

void settings_reset() {
	settings = (Settings)SETTINGS_DEFAULTS;
	preset = (Preset)PRESET_DEFAULTS;
	settings_write();
}
