#pragma once
#include "stdio.h"

#define print(x) fputs(x, stdout)
#define println(x) puts(x)

#define VERBOSE(x) puts("\e[0;34m" x "\e[0m") //blue
#define DEBUG(x)   puts("\e[0;32m" x "\e[0m") //green
#define INFO(x)    puts("\e[0;37m" x "\e[0m") //white
#define WARN(x)    puts("\e[0;33m" x "\e[0m") //yellow
#define ERR(x)     puts("\e[0;31m" x "\e[0m") //red
#define CRIT(x)    puts("\e[1;31m" x "\e[0m") //bright red

#define VT_CLEAR "\e\[H\e[2J"
#define VT_BS "\e[D \e[D"
#define TERM_PROMPT "\e[1;36m> \e[0m"

typedef void (*CommandHandler)(void);
typedef struct {
	const char *name;
	CommandHandler handler;
} Command;

void term_help();
void term_read();

static inline void term_clear() {
	print(VT_CLEAR);
}

static inline void term_prompt() {
	print(TERM_PROMPT);
}
