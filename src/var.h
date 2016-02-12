#pragma once
#include <avr/pgmspace.h>
#include "main.h"

typedef enum {
	Integer
} VarType;

typedef struct {
	const char *name;
	VarType type;
	union {
		int *integer;
	};
} Var;


const char str_delay[] PROGMEM = "delay";

Var vars[] = {
	{str_delay, Integer, {&delay}}
};

