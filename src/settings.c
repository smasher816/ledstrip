#include "avr/eeprom.h"
#include "settings.h"

EEMEM Settings eeprom = SETTINGS_DEFAULTS;
Settings settings;

void settings_read() {
	eeprom_read_block(&settings, 0x00, sizeof(Settings));
}

void settings_write() {
	eeprom_update_block(&settings, 0x00, sizeof(Settings));
}

void settings_reset() {
	settings = (Settings)SETTINGS_DEFAULTS;
	settings_write();
}
