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
    commands[commandCount].help = help;
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
        char *toReturn = "Help menu:\n";
        for (int i = 0; i < commandCount; i++)
            toReturn = sPrintf("%s\n%s", toReturn, commands[i].help);
        toReturn = sPrintf("::%s\nEnd of help menu.", toReturn);
        toReturn = realloc(toReturn, strlen(toReturn) * sizeof(char), strlen(toReturn) * sizeof(char)); // creates a string not in freePrints.
        freePrints();                                                                                   // frees all non-returned auxiliary strings created by sPrintf
        addToAllocated(toReturn);                                                                       // ensures toReturn can be freed by nstdlib upon next call to freePrints
        return toReturn;                                                                                // only string that remains in malloc is the one that was returned
    }
    for (int i = 0; i < commandCount; i++) // otherwise, a single matching help menu is printed if it exists.
    {
        if (isFirstWord(commandParameters, commands[i].code))
            return sPrintf("Help menu:\n%s\nEnd of help menu.", commands[i].help);
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
        setSnakeDrawing(BIG_DRAW_SIZE, goomba, goomba, goomba, goomba, marioItem, marioItemColorMap);
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
    /* comentado temporalmente porque no alcanza el espacio
    else if (strcmp(commandParameters, "pong"))
    {
        setBackgroundArray(pongArray);
        setBackgroundColorMap(pongColorMap);
        setSnakeDrawing(DRAW_SIZE, classicHeadUp, classicOther, classicTail, 0, classicApple, appleColorMap);
        setDrawOptions(0, 0, 1, 0);
        matchFlag = 1;
    }
    */
    else if (strcmp(commandParameters, "camelot"))
    {
        setBackgroundArray(camelotArray);
        setBackgroundColorMap(camelotColorMap);
        setSnakeDrawing(BIG_DRAW_SIZE, stone, stone, stone, stone, excalibur, excaliburColorMap);
        setDrawOptions(1, 0, 0, 0);
        matchFlag = 1;
    }
    if (matchFlag)
        return "Theme set.";
    return sPrintf("No theme matching %s was found.", commandParameters);
}

char *changehighlightColor(char *commandParameters, char *mustRedraw)
{
    if (!((*commandParameters >= '0' && *commandParameters <= '9') || (*commandParameters >= 'A' && commandParameters <= 'F') || (*commandParameters >= 'a' && *commandParameters <= 'f')))
        return "Hex value given not valid.";
    setHighlightColor(atoiHex(commandParameters)); // will read until an invalid character is found or 8 characters have been read. If an invalid character was found, what was read so far will be set as the color.
    *mustRedraw = 1;
    return "Highlight color set.";
}
char *changeLetterColor(char *commandParameters, char *mustRedraw)
{
    if (!((*commandParameters >= '0' && *commandParameters <= '9') || (*commandParameters >= 'A' && commandParameters <= 'F') || (*commandParameters >= 'a' && *commandParameters <= 'f')))
        return "Hex value given not valid.";
    uint64_t hex = 0;
    setLetterColor(atoiHex(commandParameters));
    *mustRedraw = 1;
    return "Letter color set";
}
char *changeLetterSize(char *commandParameters, char *mustRedraw)
{
    uint64_t newSize = atoi(commandParameters);
    if (newSize == 0 || newSize >= MAX_LETTER_SIZE)
        return "Invalid letter size.";
    resize((double)newSize);
    *mustRedraw = 1;
    return "Size set";
}
char *clearTheShell(char *commandParameters, char *mustRedraw)
{
    uint64_t toClear;
    if ((toClear = atoi(commandParameters)))
    {
        *mustRedraw = 1;
        warpNLines(toClear);
    }
    else
        clearShell();
    return "";
}
void initializeCommands()
{
    addCommand("help", "Help display for help module.\nFormat(s): 'help' | 'help' [MODULE_NAME]\nDisplays the help displays for all modules or the module specified.", getHelp);
    addCommand("snake", "Help display for snake module.\nFormat(s): 'snake' | 'snake [PLAYERS]'\nStarts the snake module. If PLAYERS is greater than two, game will be initialized with two players. If no PLAYERS parameter is given, game will be initialized with one player.", startSnake);
    addCommand("set-theme", "Help display for set theme module.\nFormat: 'set-theme [THEME]'\nSets the theme of the snake game to the specified theme.\nCurrently supported themes are:\nmario windows camelot creation pong", setSnakeTheme);
    addCommand("set-size", "Help display for set size module.\nFormat: 'set-size [NUMBER]'\nSets the size of the shell to the specified integer.", changeLetterSize);
    addCommand("set-letter-color", "Help display for set letter color module.\nFormat: 'set-letter-color [HEX_COLOR]'\nSets the letter color of the shell to the specified integer.", changeLetterColor);
    addCommand("set-highlight-color", "Help display for set highlight color.\nFormat: 'set-highlight-color [HEX_COLOR]'\nSets the highlight color of the shell to the specified integer.", changehighlightColor);
    addCommand("clear", "Help display for clear module. \n Format(s): 'clear' | 'clear [LINES]'\nClears the shell or the number or shifts up the number of lines indicated.", clearTheShell);
}