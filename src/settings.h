#pragma once
#include "main.h"
#include "msgeq7.h"
#include "input.h"
#include "led.h"

#define VERSION 1

#define SETTINGS_DEFAULTS { \
	0, \
	1, \
	{0xFF,0xFF,0xFF}, \
	{0xFF,0xFF,0xFF}, \
	0, \
	255, \
	255, \
	10, \
	20, \
	F_BASS, \
	30, \
	100, \
	10, \
}

#define PRESET_DEFAULTS { \
	MODE_MUSIC, \
	{&settings.hue, 0, 255}, \
	{&settings.music_min_brightness, 50, 100}, \
}

typedef struct {
	uint8_t preset;
	uint8_t preset_count;
	rgb_t correction;
	rgb_t temperature;
	uint8_t hue;
	uint8_t saturation;
	uint8_t brightness;
	int delay;
	uint8_t msgeq7_min;
	uint8_t music_frequency;
	uint8_t music_fps;
	uint8_t music_sensitivity;
	uint8_t music_min_brightness;
} Settings;

typedef struct {
	uint8_t mode;
	Input input1;
	Input input2;
} Preset;

extern Settings settings;
extern Preset preset;

void settings_read();
void settings_preset(int i);
void settings_write();
void settings_reset();
