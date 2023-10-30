#ifndef CMNDP_H
#define CMNDP_H
#define COMMAND_COUNT 7
#include <stdint.h>
#include "./../../nstdlib/nstdlib.h"
#include "./../../snake/snake.h"
#include "./../../window/backgroundArrays.h"
/**
 * Currently:
 * set highlight and letter colors
 * warp up
 * change letter size
 * change command prompt
 * snake
 * sing
 * exit
 * help
 */
typedef struct command
{
    char *code;
    char *help;
    char *(*action)(char *);
} command;

void addCommand(char *commandCode, char *help, char *(*action)(char *));
char *handleCommand(char *command);

char *getHelp(char *commandParameters);

#endif
