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
typedef struct command
{
    char *code;
    char *help;
    char *(*action)(char *, char *);
} command;

void addCommand(char *commandCode, char *help, char *(*action)(char *, char *));
char *handleCommand(char *command, char *mustRedraw);

#endif
