#ifndef CMND_H
#define CMND_H
typedef struct stringOrFd
{
    char *s;
    int fd;
} stringOrFd;
typedef enum displayStyles
{
    APPEND = 0,
    REDRAW_ONCE,
    REDRAW_ALWAYS,
} displayStyles;
stringOrFd handleCommand(char *command, displayStyles *displayStyle); // All it does.
void initializeCommands();
void freeCommands();

#endif
