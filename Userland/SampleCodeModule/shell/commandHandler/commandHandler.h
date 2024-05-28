#ifndef CMND_H
#define CMND_H
#include "../shell.h"

char *handleCommand(shell s, commandSet c, char *command, char *mustRedraw); // All it does.
commandSet initializeCommands();
void freeCommands();

#endif
