#pragma once

void mode_music();

typedef struct {
	uint8_t hue;
	uint8_t saturation;
	uint8_t frequency;
	uint8_t fps;
	uint8_t sensitivity;
	uint8_t min_brightness;
} MusicConfig;
