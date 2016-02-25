#pragma once
#include "main.h"
#include "msgeq7.h"

#define VERSION 1

#define SETTINGS_DEFAULTS { \
	MODE_MUSIC, \
	0, \
	10, \
	20, \
	F_BASS, \
	30, \
	100, \
}

typedef struct {
	uint8_t mode;
	uint8_t hue;
	int delay;
	uint8_t msgeq7_min;
	uint8_t music_frequency;
	uint8_t music_fps;
	uint8_t music_sensitivity;
} Settings;

extern Settings settings;

void settings_read();
void settings_write();
void settings_reset();
