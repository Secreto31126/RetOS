#ifndef CMNDP_H
#define CMNDP_H
#define COMMAND_COUNT 7
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Any module wrapper must adjust to this definition
 *
 */
typedef moduleData (*action_t)(moduleData, displayStyles *);
/**
 * @brief For referencing a module's wrapper
 *
 */
typedef struct command {
  char *code;
  char *help;
  action_t action;
} command;
/**
 * @brief Adds a command to the list of commands stored in the commandHandler
 *
 * @param commandCode The string that identifies the module
 * @param help A brief explanation of the module's use and function
 * @param action Function pointer to the module's action_t wrapper
 */
void addCommand(char *commandCode, char *help, action_t action);

#endif
