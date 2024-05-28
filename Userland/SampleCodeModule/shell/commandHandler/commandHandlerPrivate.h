#ifndef CMNDP_H
#define CMNDP_H
#define COMMAND_COUNT 7
#include <stdint.h>
#include "../shell.h"
/**
 * Currently:
 * set highlight and letter colors .
 * warp up
 * change letter size .
 * change command prompt
 * play snake .
 * snake themes .
 * sing
 * exit
 * help .
 */

void addCommand(commandSet c, char *commandCode, char *help, action_t action);
char *handleCommand(shell s, commandSet c, char *command, char *mustRedraw);

#endif
