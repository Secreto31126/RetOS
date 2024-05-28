#ifndef CMND_H
#define CMND_H
typedef struct stringOrFd
{
    char *s;
    int fd;
} stringOrFd;
stringOrFd handleCommand(char *command, char *mustRedraw); // All it does.
void initializeCommands();
void freeCommands();

#endif
