#ifndef CMNDP_H
#define CMNDP_H

typedef struct command
{
    char *code;
    char *(action(char));
} command;

#endif
