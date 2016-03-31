#pragma once
#include <stdint.h>
#include "vars.h"
#include "mode/static.h"
#include "mode/cycle.h"
#include "mode/music.h"

typedef union {
	StaticConfig hsv;
	CycleConfig cycle;
	MusicConfig music;
} Config;

typedef struct {
	uint8_t mode;
	Config config;
} Mode;

typedef void (*ModeHandler)(Config*);

typedef enum {
	MODE_STATIC,
	MODE_CYCLE,
	MODE_MUSIC,
	MODE_LAST
} Modes;

extern ModeHandler mode_handlers[];
extern VarGroup mode_vars[];
