#include "commandHandler.h"
#include "commandHandlerPrivate.h"
#include "./../../snake/snake.h"
#define BLOCK 10

static command *commands;
static uint64_t commandCount = 0;

void initializeCommands()
{
    if (!(commandCount % BLOCK))
        commands = realloc(commands, ((commandCount / BLOCK + 1) * BLOCK) * sizeof(command), commandCount * sizeof(command));
}
void freeCommands()
{
    free(commands);
}
void addCommand(char *commandCode, char *help, char *(*commandAction)(char *))
{
    if (!(commandCount % BLOCK))
        commands = realloc(commands, commandCount * sizeof(command), (commandCount + BLOCK) * sizeof(command));
    commands[commandCount].code = commandCode;
    commands[commandCount].action = commandAction;
    commandCount++;
}
char *handleCommand(char *command)
{
    command = shiftToWord(command);
    if (strcmpHandleWhitespace(command, "") || strcmpHandleWhitespace(command, " "))
        return "";
    for (int i = 0; i < commandCount; i++)
    {
        if (isPrefix(command, commands[i].code))
        {
            return commands[i].action(command + strlen(commands[i].code)); // Passes rest of command (parameters) to the defined action. Action is responsible for confirming valid parameters.
        }
    }
    return "Command was not recognized";
}
// prints will be freed after calling this function. returned string is not freed.
char *getHelp(char *commandParameters)
{
    commandParameters = shiftToWord(commandParameters);
    if (strcmpHandleWhitespace("", commandParameters))
    {
        char *toReturn = "";
        for (int i = 0; i < commandCount; i++)
            toReturn = sPrintf("%s\n\n%s", toReturn, commands[i].help);
        toReturn = realloc(toReturn, strlen(toReturn) * sizeof(char), strlen(toReturn) * sizeof(char)); // creates a string not in freePrints.
        freePrints();
        addToAllocated(toReturn);
        return toReturn; // only allocated
    }
    for (int i = 0; i < commandCount; i++)
        if (strcmpHandleWhitespace(commandParameters, commands[i].code))
            return commands[i].help;
}

char *startSnake(char *commandParameters)
{
    commandParameters = shiftToWord(commandParameters);
    char *formatString = "Player %d won. Returning to shell";
    if (strcmpHandleWhitespace("", commandParameters) || strcmpHandleWhitespace("1", commandParameters))
        return sPrintf(formatString, playSnake(1));
    if (strcmpHandleWhitespace("2", commandParameters))
        return sPrintf(formatString, playSnake(2));
    return "Invalid snake parameter";
}

char *setSnakeTheme(char *commandParameters)
{
    commandParameters = shiftToWord(commandParameters);
}