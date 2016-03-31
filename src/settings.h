#pragma once
#include "main.h"
#include "msgeq7.h"
#include "input.h"
#include "led.h"
#include "mode.h"
#include "vars.h"

#define VERSION 1
#define MODE_COUNT 2

#define SETTINGS_DEFAULTS { \
	0, \
	1, \
	255, \
	{0xFF,0xFF,0xFF}, \
	{0xFF,0xFF,0xFF}, \
	20 \
}

#define PRESET_DEFAULTS { \
	{ \
		{MODE_STATIC, {.hsv = {0, 255, 255}}}, \
		{MODE_MUSIC, {.music = {F_BASS, 30, 100, 10}}} \
	}, \
	{&preset.modes[0].config.hsv.hue, 0, 255}, \
	{&preset.modes[1].config.music.min_brightness, 50, 100}, \
}

typedef struct {
	uint8_t preset;
	uint8_t preset_count;
	uint8_t brightness;
	rgb_t correction;
	rgb_t temperature;
	uint8_t msgeq7_min;
} Settings;

typedef struct {
	Mode modes[MODE_COUNT];
	Input input1;
	Input input2;
} Preset;

extern Settings settings;
extern Preset preset;
extern VarGroup vars_settings;

void settings_read();
void settings_preset(uint8_t i);
void settings_write();
void settings_reset();
