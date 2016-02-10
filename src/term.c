#include "term.h"
#include "uart.h"

#define BUFFER_SIZE 16

char buffer[BUFFER_SIZE];
int bufpos;

int term_read() {
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
				puts(buffer);
				term_prompt();
				return 1;
			} else if (c=='\b' || c==127) {
				if (bufpos > 0) {
					fputs(VT_BS, stdout);
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
	return 0;
}
