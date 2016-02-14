#pragma once
#include <avr/pgmspace.h>
#include "term.h"
#include "util.h"
#include "settings.h"

#define ARRAYLEN(a) (sizeof(a)/sizeof(a[0]))

typedef void (*CommandHandler)(int argc, char *argv[]);

typedef struct {
	const char *name;
	CommandHandler handler;
} Command;


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
	{str_set,     set},
	{str_get,     get},
	{str_read,    settings_read},
	{str_write,   settings_write},
	{str_default, settings_reset},
};
