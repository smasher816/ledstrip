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

typedef struct {
	const char *name;
	uint8_t count;
	Var *vars[];
} VarGroup;

void var_set(int argc, char *argv[]);
void var_get(int argc, char *argv[]);
