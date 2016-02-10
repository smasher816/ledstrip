#pragma once
#include "term.h"
#include "main.h"

Command commands[] = {
	{"help",  term_help},
	{"clear", term_clear},
	{"version", version},
	{"ram", ram},
	{"reset", reset},
	{"led", toggle_led}
};
