#pragma once
#include "stdio.h"

#define VT_CLEAR "\e\[H\e[2J"
#define VT_BS "\e[D \e[D"
#define TERM_PROMPT "\e[1;36m> \e[0m"

int term_read();
static inline void term_clear() {
	fputs(VT_CLEAR, stdout);
}
static inline void term_prompt() {
	fputs(TERM_PROMPT, stdout);
}
