#ifndef CMND_H
#define CMND_H
typedef struct moduleData
{
    char *s;
    int fd;
    int writeFd;
    int pid;
} moduleData;
typedef enum displayStyles
{
    APPEND = 0,
    REDRAW_ONCE,
    REDRAW_ALWAYS,
} displayStyles;
moduleData handleCommand(char *command, displayStyles *displayStyle); // All it does.
void initializeCommands();
void freeCommands();

#endif
