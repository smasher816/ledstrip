#pragma once

typedef enum {
	Integer,
	Byte,
} VarType;

typedef struct {
	const char *name;
	VarType type;
	void *value;
} Var;


extern Var vars[];
extern uint8_t var_count;

void var_set(int argc, char *argv[]);
void var_get(int argc, char *argv[]);
