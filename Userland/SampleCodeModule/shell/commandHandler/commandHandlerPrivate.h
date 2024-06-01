#ifndef CMNDP_H
#define CMNDP_H
#define COMMAND_COUNT 7
#include <stdint.h>
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
typedef moduleData (*action_t)(moduleData, displayStyles *);
typedef struct command
{
    char *code;
    char *help;
    action_t action;
} command;

void addCommand(char *commandCode, char *help, action_t action);

#endif
