#pragma once

typedef void (*CommandHandler)(int argc, char *argv[]);

typedef struct {
	const char *name;
	CommandHandler handler;
} Command;

void list_commands();
int command_exec(char *cmd, uint8_t argc, char *argv[]);
