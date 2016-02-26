#pragma once
#include <avr/pgmspace.h>
#include "settings.h"

typedef enum {
	Integer,
	Byte,
} VarType;

typedef struct {
	const char *name;
	VarType type;
	void *value;
} Var;


const char str_mode[]  PROGMEM = "preset";
const char str_hue[]   PROGMEM = "hue";
const char str_delay[] PROGMEM = "delay";
const char str_min[]   PROGMEM = "min";
const char str_freq[]  PROGMEM = "freq";
const char str_fps[]   PROGMEM = "fps";
const char str_sens[]  PROGMEM = "sens";

Var vars[] = {
	{str_mode,  Byte,    &settings.preset},
	{str_hue,   Byte,    &settings.hue},
	{str_delay, Integer, &settings.delay},
	{str_min,   Byte,    &settings.msgeq7_min},
	{str_freq,  Byte,    &settings.music_frequency},
	{str_fps,   Byte,    &settings.music_fps},
	{str_sens,  Byte,    &settings.music_sensitivity}
};

