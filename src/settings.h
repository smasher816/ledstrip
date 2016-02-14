#pragma once

#define VERSION 1

#define SETTINGS_DEFAULTS { \
	10 \
}

typedef struct {
	int delay;
} Settings;

extern Settings settings;

void settings_read();
void settings_write();
void settings_reset();
