#pragma once
#include "main.h"
#include "msgeq7.h"
#include "input.h"
#include "led.h"
#include "mode.h"

#define VERSION 1

#define SETTINGS_DEFAULTS { \
	0, \
	1, \
	255, \
	{0xFF,0xFF,0xFF}, \
	{0xFF,0xFF,0xFF}, \
	20 \
}

#define PRESET_DEFAULTS { \
	MODE_MUSIC, \
	{&preset.config.music.hue, 0, 255}, \
	{&preset.config.music.min_brightness, 50, 100}, \
	{.music = {255, 255, F_BASS, 30, 100, 10}} \
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
	uint8_t mode;
	Input input1;
	Input input2;
	Config config;
} Preset;

extern Settings settings;
extern Preset preset;

void settings_read();
void settings_preset(uint8_t i);
void settings_write();
void settings_reset();
