#include "commandHandler.h"
#include "commandHandlerPrivate.h"
#include "./../../snake/snake.h"
#include "./../../nstdlib/nstdlib.h"
#include "./../../snake/snake.h"
#include "./../../snake/drawings/backgroundArrays.h"
#include "./../../snake/drawings/snakeDrawings.h"
#include "./../shell.h"
#define BLOCK 10
#define MAX_LETTER_SIZE 10

static command *commands;
static uint64_t commandCount = 0;

void freeCommands()
{
    free(commands);
}
void addCommand(char *commandCode, char *help, char *(*commandAction)(char *, char *))
{
    if (!(commandCount % BLOCK))
        commands = realloc(commands, commandCount * sizeof(command), (commandCount + BLOCK) * sizeof(command));
    commands[commandCount].code = commandCode;
    commands[commandCount].action = commandAction;
    commandCount++;
}
char *handleCommand(char *command, char *mustRedraw)
{
    command = shiftToWord(command);
    if (strcmpHandleWhitespace(command, "") || strcmpHandleWhitespace(command, " ")) // if command is empty or whitespace, it is ignored
        return "";
    for (int i = 0; i < commandCount; i++)
    {
        if (isFirstWord(commands[i].code, command))
        {
            return commands[i].action(shiftToWord(command + strlen(commands[i].code)), mustRedraw); // Passes rest of command (the parameters) to the defined action. Skips to next word in case there is whitespace between the command and its parameters. Action is responsible for confirming valid parameters.
        }
    }
    return "Command was not recognized";
}
// prints will be freed after calling this function. returned string is not freed.
char *getHelp(char *commandParameters, char *mustRedraw)
{
    if (strcmpHandleWhitespace("", commandParameters)) // if the command is just 'help', all help menus are printed
    {
        char *toReturn = "";
        for (int i = 0; i < commandCount; i++)
            toReturn = sPrintf("%s\n\n%s", toReturn, commands[i].help);
        toReturn = sPrintf("%s\n\nEnd of help menu.", toReturn);
        toReturn = realloc(toReturn, strlen(toReturn) * sizeof(char), strlen(toReturn) * sizeof(char)); // creates a string not in freePrints.
        freePrints();                                                                                   // frees all non-returned auxiliary strings created by sPrintf
        addToAllocated(toReturn);                                                                       // ensures toReturn can be freed by nstdlib upon next call to freePrints
        return toReturn;                                                                                // only string that remains in malloc is the one that was returned
    }
    for (int i = 0; i < commandCount; i++) // otherwise, a single matching help menu is printed if it exists.
    {
        if (isFirstWord(commandParameters, commands[i].code))
            return sPrintf("%s\n\nEnd of help menu.", commands[i].help);
    }
    return sPrintf("No help menu that matches '%s' was found.", commandParameters); // sPrintf automatically adds created string to list of strings to be freed by freePrints()
}

char *startSnake(char *commandParameters, char *mustRedraw)
{
    char *formatString = "Player %d won. Returning to shell";
    if (strcmp("", commandParameters) || strcmp("1", commandParameters))
    {
        *mustRedraw = 1;
        return sPrintf(formatString, playSnake(1));
    }
    if (strcmp("2", commandParameters))
    {
        *mustRedraw = 1;
        return sPrintf(formatString, playSnake(2));
    }
    return "Invalid snake parameter";
}

char *setSnakeTheme(char *commandParameters, char *mustRedraw)
{
    char matchFlag = 0;
    if (strcmp(commandParameters, "creation"))
    {
        setBackgroundArray(creationArray);
        setBackgroundColorMap(creationColorMap);
        setSnakeDrawing(DRAW_SIZE, classicHeadUp, classicOther, classicTail, 0, classicApple, appleColorMap);
        setDrawOptions(0, 0, 1, 0);
        matchFlag = 1;
    }
    else if (strcmp(commandParameters, "mario"))
    {
        setBackgroundArray(marioArray);
        setBackgroundColorMap(marioColorMap);
        setSnakeDrawing(BIG_DRAW_SIZE, goomba, goomba, goomba, goomba, goomba, marioItemColorMap);
        setDrawOptions(1, 0, 0, 0);
        matchFlag = 1;
    }
    else if (strcmp(commandParameters, "windows"))
    {
        setBackgroundArray(windowsArray);
        setBackgroundColorMap(windowsColorMap);
        setSnakeDrawing(DRAW_SIZE, classicHeadUp, classicOther, classicTail, 0, classicApple, appleColorMap);
        setDrawOptions(0, 0, 1, 0);
        matchFlag = 1;
    }
    else if (strcmp(commandParameters, "pong"))
    {
        setBackgroundArray(pongArray);
        setBackgroundColorMap(pongColorMap);
        setSnakeDrawing(DRAW_SIZE, classicHeadUp, classicOther, classicTail, 0, classicApple, appleColorMap);
        setDrawOptions(0, 0, 1, 0);
        matchFlag = 1;
    }
    if (matchFlag)
        return "Theme set.";
    return sPrintf("No theme matching %s was found. Available themes are 'mario', 'creation', 'windows', and 'pong'", commandParameters);
}

char *changeighlightColor(char *commandParameters, char *mustRedraw)
{
    if (!((*commandParameters >= '0' && *commandParameters <= '9') || (*commandParameters >= 'A' && commandParameters <= 'F') || (*commandParameters >= 'a' && *commandParameters <= 'f')))
        return "Hex value given not valid.";
    setHighlightColor(atoiHex(commandParameters)); // will read until an invalid character is found or 8 characters have been read. If an invalid character was found, what was read so far will be set as the color.
    return "Highlight color set.";
}
char *changeLetterColor(char *commandParameters, char *mustRedraw)
{
    if (!((*commandParameters >= '0' && *commandParameters <= '9') || (*commandParameters >= 'A' && commandParameters <= 'F') || (*commandParameters >= 'a' && *commandParameters <= 'f')))
        return "Hex value given not valid.";
    uint64_t hex = 0;
    setLetterColor(atoiHex(commandParameters));
    return "Letter color set";
}
char *changeLetterSize(char *commandParameters, char *mustRedraw)
{
    uint64_t newSize = atoi(commandParameters);
    if (newSize == 0 || newSize >= MAX_LETTER_SIZE)
        return "Invalid letter size.";
    setSize((double)newSize);
    return "Size set";
}
void initializeCommands()
{
    addCommand("help ", "Help display for help module.\n\nFormat(s): 'help' | 'help' [MODULE_NAME]\n\nDisplays the help displays for all modules or the module specified.", getHelp);
    addCommand("snake ", "Help display for snake module.\n\nFormat(s): 'snake' | 'snake [PLAYERS]'\n\nStarts the snake module. If PLAYERS is greater than two, game will be initialized with two players. If no PLAYERS parameter is given, game will be initialized with one player.", startSnake);
}