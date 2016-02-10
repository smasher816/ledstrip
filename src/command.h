#pragma once
#include "term.h"
#include "main.h"

Command commands[] = {
	{"help",  term_help},
	{"clear", term_clear},
	{"led", toggle_led}
};
