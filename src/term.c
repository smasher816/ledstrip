#include <string.h>
#include "term.h"
#include "uart.h"
#include "print.h"
#include "command.h"

#define BUFFER_SIZE 16
#define MAX_PARAMS 3

char buffer[BUFFER_SIZE];
int bufpos;

void term_help() {
	println("Valid commands are:");
	for(unsigned int i=0; i<ARRAYLEN(commands); i++) {
		puts_P(commands[i].name);
	}
}

void term_process() {
	int argc;
	char *cmd, *argv[MAX_PARAMS];

	cmd = strtok(buffer, " ");
	for (argc=0; (argv[argc]=strtok(NULL, " "))!=NULL; argc++);

	for(unsigned int i=0; i<ARRAYLEN(commands); i++) {
		if (strcmp_P(cmd, commands[i].name)==0) {
			commands[i].handler(argc, argv);
			return;
		}
	}
	ERR("Unknown command");
}

void term_read() {
	char c;
	static int k = 0;
	while (uart_available()) {
		c = getchar();
		//Handle escape sequences
		if (c=='\e') {
			k = 1;
		} else if (c=='[' && k==1) {
			k = 2;
		} else if (k == 2) {
			//Arrow keys and stuff. Ignore for now
			k = 0;
		} else {
			//Non-escape text
			k = 0;
			if (c=='\r' || c=='\n') {
				putchar('\n');
				buffer[bufpos] = '\0';
				bufpos = 0;
				term_process();
				term_prompt();
			} else if (c=='\b' || c==127) {
				if (bufpos > 0) {
					print(VT_BS);
					bufpos--;
				}
			} else if (c>=32 && c<=126) {
				if (bufpos < BUFFER_SIZE) {
					putchar(c);
					buffer[bufpos++] = c;
				}
			} else {
				//Other special characters. Ignore.
			}
		}
	}
}
