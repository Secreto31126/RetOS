#include "commandHandler.h"
#include "commandHandlerPrivate.h"
#include "./../../snake/snake.h"
#include "./../../nstdlib/nstdlib.h"
#include "./../../snake/snake.h"
#include "./../../snake/drawings/backgroundArrays.h"
#include "./../../snake/drawings/snakeDrawings.h"
#include "./../shell.h"
#include "./../../piano/piano.h"
#include "./../../piano/sound.h"
#define BLOCK 5
#define MAX_LETTER_SIZE 4

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
        uint64_t len = 0, i;
        for (i = 0; i < commandCount; i++)
            len += strlen(commands[i].help);
        char *display = malloc((len + i * 2 + 1) * sizeof(char));
        char *aux = display;
        for (i = 0; i < commandCount; i++)
        {
            aux += concatFrom(aux, commands[i].help);
            aux += concatFrom(aux, "\n\n");
        }
        addToAllocated(display);
        return display;
    }
    for (int i = 0; i < commandCount; i++) // otherwise, a single matching help menu is printed if it exists.
    {
        if (isFirstWord(commandParameters, commands[i].code))
            return commands[i].help;
    }
    return sPrintf("No help menu that matches '%s' was found.", commandParameters); // sPrintf automatically adds created string to list of strings to be freed by freePrints()
}

char *startSnake(char *commandParameters, char *mustRedraw)
{
    char *formatString = "Player %d won. Returning to shell";
    int i;
    if (strcmp("", commandParameters) || (i = atoi(commandParameters)) == 1)
    {
        *mustRedraw = 1;
        return sPrintf(formatString, playSnake(1));
    }
    if (i) // if 0, either input was 0 or invalid. Either way, input is not valid
    {
        *mustRedraw = 1;
        return sPrintf(formatString, playSnake(i > 3 ? 3 : i)); // we support a third player, it is controlled with the spacebar
    }
    return "Invalid snake parameter";
}

char *setSnakeTheme(char *commandParameters, char *mustRedraw)
{
    char matchFlag = 0;
    if (strcmp(commandParameters, "windows"))
    {
        setBackgroundArray((char *)windowsArray);
        setBackgroundColorMap((HexColor *)windowsColorMap);
        setSnakeDrawing(DRAW_SIZE, (char *)classicHeadUp, (char *)classicOther, (char *)classicTail, (char *)classicTurn, (char *)classicApple, (HexColor *)appleColorMap);
        setDrawOptions(0, 0, 1, 0);
        matchFlag = 1;
    }
    else if (strcmp(commandParameters, "mario"))
    {
        setBackgroundArray((char *)marioArray);
        setBackgroundColorMap((HexColor *)marioColorMap);
        setSnakeDrawing(BIG_DRAW_SIZE, (char *)goomba, (char *)goomba, (char *)goomba, (char *)pipe, (char *)marioItem, (HexColor *)marioItemColorMap);
        setDrawOptions(1, 0, 1, 1);
        matchFlag = 1;
    }
    else if (strcmp(commandParameters, "pong"))
    {
        setBackgroundArray((char *)pongArray);
        setBackgroundColorMap((HexColor *)pongColorMap);
        setSnakeDrawing(DRAW_SIZE, (char *)classicHeadUp, (char *)classicOther, (char *)classicTail, (char *)classicTurn, (char *)classicApple, (HexColor *)appleColorMap);
        setDrawOptions(0, 0, 1, 0);
        matchFlag = 1;
    }
    else if (strcmp(commandParameters, "creation"))
    {
        setBackgroundArray((char *)creationArray);
        setBackgroundColorMap((HexColor *)creationColorMap);
        setSnakeDrawing(DRAW_SIZE, (char *)classicHeadUp, (char *)classicOther, (char *)classicTail, (char *)classicTurn, (char *)classicApple, (HexColor *)appleColorMap);
        setDrawOptions(0, 0, 1, 0);
        matchFlag = 1;
    }
    else if (strcmp(commandParameters, "camelot"))
    {
        setBackgroundArray((char *)camelotArray);
        setBackgroundColorMap((HexColor *)camelotColorMap);
        setSnakeDrawing(BIG_DRAW_SIZE, (char *)stone, (char *)stone, (char *)stone, (char *)catapult, (char *)excalibur, (HexColor *)excaliburColorMap);
        setDrawOptions(1, 0, 1, 1);
        matchFlag = 1;
    }
    else if (strcmp(commandParameters, "idyllic"))
    {
        setBackgroundArray((char *)idyllicArray);
        setBackgroundColorMap((HexColor *)idyllicColorMap);
        setSnakeDrawing(BIG_DRAW_SIZE, (char *)wyvHead, (char *)wyvBody, (char *)wyvTail, (char *)wyvTurn, (char *)guide, (HexColor *)guideColorMap);
        setDrawOptions(1, 1, 1, 1);
        matchFlag = 1;
    }
    if (matchFlag)
        return "Theme set.";
    return sPrintf("No theme matching %s was found.", commandParameters);
}

char *changehighlightColor(char *commandParameters, char *mustRedraw)
{
    if (!((*commandParameters >= '0' && *commandParameters <= '9') || (*commandParameters >= 'A' && *commandParameters <= 'F') || (*commandParameters >= 'a' && *commandParameters <= 'f')))
        return "Hex value given not valid.";
    setHighlightColor(atoiHex(commandParameters)); // will read until an invalid character is found or 8 characters have been read. If an invalid character was found, what was read so far will be set as the color.
    *mustRedraw = 1;
    return "Highlight color set.";
}
char *changeLetterColor(char *commandParameters, char *mustRedraw)
{
    if (!((*commandParameters >= '0' && *commandParameters <= '9') || (*commandParameters >= 'A' && *commandParameters <= 'F') || (*commandParameters >= 'a' && *commandParameters <= 'f')))
        return "Hex value given not valid.";
    // uint64_t hex = 0;
    setLetterColor(atoiHex(commandParameters));
    *mustRedraw = 1;
    return "Letter color set";
}
char *changeLetterSize(char *commandParameters, char *mustRedraw)
{
    uint64_t newSize = atoi(commandParameters);
    if (newSize < 0 || newSize > MAX_LETTER_SIZE)
        return "Invalid letter size.";
    if (!newSize) // would be nice to actually handle doubles, however, no parseDouble function can be made (reasonably easily) if SSE registers cannot be used to return the value.
        resize(0.5);
    else
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
char *readMeTheDump(char *commandParameters, char *mustRedraw)
{
    char *c = getDumpString();
    if (strcmp(c, ""))
        return "No dump generated. Press 'alt' to generate a dump of the instant of pressing.";
    return sPrintf("The dump generated:\n%s", c);
}
char *playThePiano(char *commandParameters, char *mustRedraw)
{
    *mustRedraw = 1;
    startPiano();
    return "Now exiting the yellow submarine.";
}
char *singToMe(char *commandParameters, char *mustRedraw)
{
    char match = 0;
    if (strcmp(commandParameters, "imperial-march"))
    {
        imperial_march();
        match = 1;
    }
    else if (strcmp(commandParameters, "hes-a-pirate"))
    {
        hes_a_pirate();
        match = 1;
    }
    else if (strcmp(commandParameters, "outer-wilds"))
    {
        outer_wilds();
        match = 1;
    }
    else if (strcmp(commandParameters, "do-i-wanna-know"))
    {
        do_i_wanna_know();
        match = 1;
    }
    else if (strcmp(commandParameters, "sports-center"))
    {
        sports_center();
        match = 1;
    }
    else if (strcmp(commandParameters, "here-comes-the-sun"))
    {
        here_comes_the_sun();
        match = 1;
    }
    if (match)
        return "Song is over. Now it's your turn.";
    return "Found no matching song.";
}
char *repeat(char *commandParameters, char *mustRedraw)
{
    return strcmp(commandParameters, "") ? " " : commandParameters;
}
void initializeCommands()
{
    addCommand("help", "Help display for help module.\nFormat(s): 'help' | 'help' [MODULE_NAME]\nDisplays the help displays for all modules or the module specified.", getHelp);
    addCommand("snake", "Help display for snake module.\nFormat(s): 'snake' | 'snake [PLAYERS]'\nStarts the snake module. If PLAYERS is greater than two, game will be initialized with two players. If no PLAYERS parameter is given, game will be initialized with one player.", startSnake);
    addCommand("set-theme", "Help display for set theme module.\nFormat: 'set-theme [THEME]'\nSets the theme of the snake game to the specified theme.\nCurrently supported themes are:\nmario windows camelot creation pong idyllic", setSnakeTheme);
    addCommand("set-size", "Help display for set size module.\nFormat: 'set-size [NUMBER]'\nSets the size of the shell to the specified integer. Maximum accepted size is 4, anything larger is illegible. Only positive integer sizes are accepted. Size 0 will set the size to 0.5", changeLetterSize);
    addCommand("set-letter-color", "Help display for set letter color module.\nFormat: 'set-letter-color [HEX_COLOR]'\nSets the letter color of the shell to the specified integer.", changeLetterColor);
    addCommand("set-highlight-color", "Help display for set highlight color.\nFormat: 'set-highlight-color [HEX_COLOR]'\nSets the highlight color of the shell to the specified integer.", changehighlightColor);
    addCommand("clear", "Help display for clear module. \n Format(s): 'clear' | 'clear [LINES]'\nClears the shell or the number or shifts up the number of lines indicated.", clearTheShell);
    addCommand("dump", "Help display for dump module.\n Format: 'dump'\nDisplays a dump if one has been generated by pressing the 'alt' key. Indicates no dump has been generated otherwise.", readMeTheDump);
    addCommand("piano", "Help display for piano module.\nFormat: 'piano'\nStarts the piano module.\nPiano keys: z = Do, s = Do#, x = Re, d = Re#, c = mi, v = Fa, g = Fa#, b = Sol, h = Sol#, n = La, j = La#, m = Si", playThePiano);
    addCommand("sing", "Help display for sing module.\nFormat: 'sing [SONG_NAME]'\nSings a song. Currently recognized songs are:\n'imperial-march' 'hes-a-pirate' 'outer-wilds' 'do-i-wanna-know' 'sports-center' 'here-comes-the-sun'.", singToMe);
    addCommand("echo", "Help display for the echo module.\nFormat: 'echo [TO_ECHO]'\nIt repeats what you input.", repeat);
}
