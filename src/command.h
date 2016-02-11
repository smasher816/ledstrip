#pragma once
#include "term.h"
#include "main.h"

const char str_help[]    PROGMEM = "help";
const char str_clear[]   PROGMEM = "clear";
const char str_version[] PROGMEM = "version";
const char str_ram[]     PROGMEM = "ram";
const char str_reset[]   PROGMEM = "reset";

Command commands[] = {
	{str_help,      term_help},
	{str_clear,   term_clear},
	{str_version, version},
	{str_ram,     ram},
	{str_reset,   reset},
};
