#include "mode.h"

void mode_static(Config*);
void mode_cycle(Config*);
void mode_music(Config*);
void mode_beat(Config*);

void mode_none(Config *config) {}

ModeHandler mode_handlers[] = {
	mode_none,
	mode_static,
	mode_cycle,
	mode_music,
	mode_beat
};
