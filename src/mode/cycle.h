#pragma once

typedef struct {
	int delay;
	uint8_t saturation;
	uint8_t brightness;
} CycleConfig;

extern VarGroup vars_cycle;
