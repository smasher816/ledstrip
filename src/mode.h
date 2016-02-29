#pragma once
#include <stdint.h>
#include "mode/static.h"
#include "mode/cycle.h"
#include "mode/music.h"

typedef union {
	StaticConfig hsv;
	CycleConfig cycle;
	MusicConfig music;
} Config;

typedef void (*ModeHandler)(void);

typedef enum {
	MODE_STATIC,
	MODE_CYCLE,
	MODE_MUSIC,
	MODE_LAST
} Modes;

extern ModeHandler modes[];
