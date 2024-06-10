#ifndef CMND_H
#define CMND_H
#define MAX_PIPES 32

typedef struct moduleData
{
    char *s;
    int fd;
    int writeFd;
    int cPid;
} moduleData;
typedef struct commandData
{
    moduleData data;
    int *cPid;
    int cPidCount;
} commandData;
typedef enum displayStyles
{
    APPEND = 0,
    REDRAW_ONCE,
    REDRAW_ALWAYS,
    NO_STDIN,
    AS_BACKGROUND,
} displayStyles;
commandData handleCommand(char *command, displayStyles *displayStyle, int *cPidBuffer); // All it does.
void initializeCommands();
void freeCommands();

#endif
