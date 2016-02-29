#include "mode.h"

void mode_static(Config*);
void mode_cycle(Config*);
void mode_music(Config*);

ModeHandler mode_handlers[] = {
	mode_static,
	mode_cycle,
	mode_music
};
