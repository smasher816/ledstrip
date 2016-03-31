#pragma once

typedef struct {
	uint8_t frequency;
	uint8_t fps;
	uint8_t sensitivity;
	uint8_t min_brightness;
} MusicConfig;

extern VarGroup vars_music;
