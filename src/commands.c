#include <avr/pgmspace.h>
#include "term.h"
#include "util.h"
#include "vars.h"
#include "settings.h"
#include "commands.h"
#include "print.h"

#define ARRAYLEN(a) (sizeof(a)/sizeof(a[0]))

const char str_help[]    PROGMEM = "help";
const char str_clear[]   PROGMEM = "clear";
const char str_version[] PROGMEM = "version";
const char str_ram[]     PROGMEM = "ram";
const char str_reset[]   PROGMEM = "reset";
const char str_set[]     PROGMEM = "set";
const char str_get[]     PROGMEM = "get";
const char str_read[]    PROGMEM = "read";
const char str_write[]   PROGMEM = "write";
const char str_default[] PROGMEM = "default";

Command commands[] = {
	{str_help,    term_help},
	{str_clear,   term_clear},
	{str_version, version},
	{str_ram,     ram},
	{str_reset,   reset},
	{str_set,     var_set},
	{str_get,     var_get},
	{str_read,    settings_read},
	{str_write,   settings_write},
	{str_default, settings_reset},
};

void list_commands() {
	for(uint8_t i=0; i<ARRAYLEN(commands); i++) {
		puts_P(commands[i].name);
	}
}

int command_exec(char *cmd, uint8_t argc, char *argv[]) {
	puts(cmd);
	for(uint8_t i=0; i<ARRAYLEN(commands); i++) {
		if (strcmp_P(cmd, commands[i].name)==0) {
			commands[i].handler(argc, argv);
			return 1;
		}
	}
	return 0;
}
