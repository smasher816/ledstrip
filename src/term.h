#pragma once
#include "print.h"

#define VT_CLEAR "\e\[H\e[2J"
#define VT_BS "\e[D \e[D"
#define TERM_PROMPT "\e[1;36m> \e[0m"

void term_help();
void term_read();

static inline void term_clear() {
	print(VT_CLEAR);
}

static inline void term_prompt() {
	print(TERM_PROMPT);
}
