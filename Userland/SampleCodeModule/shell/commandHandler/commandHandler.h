#ifndef CMND_H
#define CMND_H
#define MAX_PIPES 32
/**
 * @brief Data pertaining to communicating with a module
 *
 */
typedef struct moduleData
{
    char *s;
    int fd;
    int writeFd;
    int cPid;
} moduleData;
/**
 * @brief Data pertaining to communicating and managing a module
 *
 */
typedef struct commandData
{
    moduleData data;
    int *cPid;
    int cPidCount;
} commandData;
/**
 * @brief For communicating how a module expects to be displayed
 *
 */
typedef enum displayStyles
{
    APPEND = 0,
    REDRAW_ONCE,
    REDRAW_ALWAYS,
    NO_STDIN,
    AS_BACKGROUND,
} displayStyles;

/**
 * @brief Executes a module from a command
 *
 * @param command A string representinf the module to be executed
 * @param displayStyle Return variable, how the module expects to be displayed
 * @param cPidBuffer Return variable, pids of all processes created for the module (does not include processes the module itself may create)
 * @return Data for the module created, either a string or an fd to read from (NULL or -1 when not created), an fd to write to the module (-1 if not created), an array of pids created for the module
 */
commandData handleCommand(char *command, displayStyles *displayStyle, int *cPidBuffer); // All it does.
/**
 * @brief Execute before using the commandHandler
 *
 */
void initializeCommands();
/**
 * @brief Execute once no longer using the commandHandler
 *
 */
void freeCommands();

#endif
