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
void addCommand(char *commandCode, char *help, stringOrFd (*commandAction)(char *, char *))
{
    if (!(commandCount % BLOCK))
        commands = realloc(commands, commandCount * sizeof(command), (commandCount + BLOCK) * sizeof(command));
    commands[commandCount].code = commandCode;
    commands[commandCount].action = commandAction;
    commands[commandCount].help = help;
    commandCount++;
}
stringOrFd handleCommand(char *command, char *mustRedraw)
{
    command = shiftToWord(command);
    if (strcmpHandleWhitespace(command, "") || strcmpHandleWhitespace(command, " "))
    { // if command is empty or whitespace, it is ignored
        stringOrFd aux = {"", 0};
        return aux;
    }
    for (int i = 0; i < commandCount; i++)
    {
        if (isFirstWord(commands[i].code, command))
        {
            return commands[i].action(shiftToWord(command + strlen(commands[i].code)), mustRedraw); // Passes rest of command (the parameters) to the defined action. Skips to next word in case there is whitespace between the command and its parameters. Action is responsible for confirming valid parameters.
        }
    }
    stringOrFd aux = {"Command was not recognized", 0};
    return aux;
}
// prints will be freed after calling this function. returned string is not freed.
stringOrFd getHelp(char *commandParameters, char *mustRedraw)
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
        stringOrFd toRet = {display, -1};
        return toRet;
    }
    for (int i = 0; i < commandCount; i++) // otherwise, a single matching help menu is printed if it exists.
    {
        if (isFirstWord(commandParameters, commands[i].code))
        {
            stringOrFd aux = {commands[i].help, 0};
            return aux;
        }
    }
    stringOrFd aux = {sPrintf("No help menu that matches '%s' was found.", commandParameters), -1}; // sPrintf automatically adds created string to list of strings to be freed by freePrints()
    return aux;
}

stringOrFd startSnake(char *commandParameters, char *mustRedraw)
{
    char *formatString = "Player %d won. Returning to shell";
    int i;
    if (strcmp("", commandParameters) || (i = atoi(commandParameters)) == 1)
    {
        *mustRedraw = 1;
        stringOrFd aux = {sPrintf(formatString, playSnake(1)), -1};
        return aux;
    }
    if (i) // if 0, either input was 0 or invalid. Either way, input is not valid
    {
        *mustRedraw = 1;
        stringOrFd aux = {sPrintf(formatString, playSnake(i > 3 ? 3 : i)), -1}; // we support a third player, it is controlled with the spacebar
        return aux;
    }
    stringOrFd aux = {"Invalid snake parameter", -1};
    return aux;
}

stringOrFd setSnakeTheme(char *commandParameters, char *mustRedraw)
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
        setSnakeDrawing(BIG_DRAW_SIZE, (char *)goomba, (char *)goomba, (char *)goomba, (char *)tube, (char *)marioItem, (HexColor *)marioItemColorMap);
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
    {
        stringOrFd aux = {"Theme set." - 1};
        return aux;
    }
    stringOrFd aux = {sPrintf("No theme matching %s was found.", commandParameters), -1};
    return aux;
}

stringOrFd changehighlightColor(char *commandParameters, char *mustRedraw)
{
    if (!((*commandParameters >= '0' && *commandParameters <= '9') || (*commandParameters >= 'A' && *commandParameters <= 'F') || (*commandParameters >= 'a' && *commandParameters <= 'f')))
    {
        stringOrFd aux = {"Hex value given not valid.", -1};
        return aux;
    }
    setHighlightColor(atoiHex(commandParameters)); // will read until an invalid character is found or 8 characters have been read. If an invalid character was found, what was read so far will be set as the color.
    *mustRedraw = 1;
    stringOrFd aux = {"Highlight color set", -1};
    return aux;
}
stringOrFd changeLetterColor(char *commandParameters, char *mustRedraw)
{
    if (!((*commandParameters >= '0' && *commandParameters <= '9') || (*commandParameters >= 'A' && *commandParameters <= 'F') || (*commandParameters >= 'a' && *commandParameters <= 'f')))
    {
        stringOrFd aux = {"Hex value given not valid.", -1};
        return aux;
    }
    // uint64_t hex = 0;
    setLetterColor(atoiHex(commandParameters));
    *mustRedraw = 1;
    stringOrFd aux = {"Letter color set", -1};
    return aux;
}
stringOrFd changeLetterSize(char *commandParameters, char *mustRedraw)
{
    uint64_t newSize = atoi(commandParameters);
    if (newSize < 0 || newSize > MAX_LETTER_SIZE)
    {
        stringOrFd aux = {"Invalid letter size.", -1};
        return aux;
    }
    if (!newSize) // would be nice to actually handle doubles, however, no parseDouble function can be made (reasonably easily) if SSE registers cannot be used to return the value.
        resize(0.5);
    else
        resize((double)newSize);
    *mustRedraw = 1;
    stringOrFd aux = {"Size set", -1};
    return aux;
}
stringOrFd clearTheShell(char *commandParameters, char *mustRedraw)
{
    uint64_t toClear;
    if ((toClear = atoi(commandParameters)))
    {
        *mustRedraw = 1;
        warpNLines(toClear);
    }
    else
        clearShell();
    stringOrFd aux = {"", -1};
    return aux;
}
stringOrFd readMeTheDump(char *commandParameters, char *mustRedraw)
{
    char *c = getDumpString();
    if (strcmp(c, ""))
    {
        stringOrFd aux = {"No dump generated. Press 'alt' to generate a dump of the instant of pressing.", -1};
        return aux;
    }
    stringOrFd aux = {sPrintf("The dump generated:\n%s", c), -1};
    return aux;
}
stringOrFd playThePiano(char *commandParameters, char *mustRedraw)
{
    *mustRedraw = 1;
    startPiano();
    stringOrFd aux = {"Now exiting the yellow submarine.", -1};
    return aux;
}
stringOrFd singToMe(char *commandParameters, char *mustRedraw)
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
    {
        stringOrFd aux = {"Song is over. Now it's your turn.", -1};
        return aux;
    }
    stringOrFd aux = {"Found no matching song.", -1};
    return aux;
}
stringOrFd repeat(char *commandParameters, char *mustRedraw)
{
    stringOrFd aux = {strcmp(commandParameters, "") ? " " : commandParameters, -1};
    return aux;
}

stringOrFd testExec(char *commandParameters, char *mustRedraw)
{
    int pipeFd[2] = {0};
    int err = pipe(pipeFd);
    if (pipeFd[0] < 0 || pipeFd[1] < 0)
    {
        stringOrFd aux = {"Pipe fds were invalid", -1};
        return aux;
    }
    if (err)
    {
        stringOrFd aux = {"Could not create pipe", -1};
        return aux;
    }
    int c_pid = fork();
    if (c_pid == -1)
    {
        stringOrFd aux = {"Could not execute command", -1};
        return aux;
    }
    if (!c_pid)
    {
        close(pipeFd[0]);
        char *aux[2] = {commandParameters, NULL};
        // execv("moduleName", aux);

        char c = pipeFd[1] + '0';
        print_sys(pipeFd[1], &c, 1);
        for (int i = 0; i < 6; i++)
        {
            sleep(1);
            print_sys(pipeFd[1], "ello", sizeof("ello") - 1);
        }

        close(pipeFd[1]);
        exit(1);
    }
    if (c_pid)
    {
        close(pipeFd[1]);
        stringOrFd aux = {NULL, pipeFd[0]};
        return aux;
    }
    stringOrFd aux = {"How did we get here?", -1};
    return aux;
}

void initializeCommands()
{
    addCommand("help", "Help display for help module.\nFormat(s): 'help' | 'help' [MODULE_NAME]\nDisplays the help displays for all modules or the module specified.", getHelp);
    addCommand("snake", "Help display for snake module.\nFormat(s): 'snake' | 'snake [PLAYERS]'\nStarts the snake module. If PLAYERS is greater than three, game will be initialized with three players. If no PLAYERS parameter is given, game will be initialized with one player. Players are controlled by 'wasd', 'ijkl', and 'v/spacebar b' key combinations.", startSnake);
    addCommand("set-theme", "Help display for set theme module.\nFormat: 'set-theme [THEME]'\nSets the theme of the snake game to the specified theme.\nCurrently supported themes are:\nmario windows camelot creation pong idyllic", setSnakeTheme);
    addCommand("set-size", "Help display for set size module.\nFormat: 'set-size [NUMBER]'\nSets the size of the shell to the specified integer. Maximum accepted size is 4, anything larger is illegible. Only positive integer sizes are accepted. Size 0 will set the size to 0.5", changeLetterSize);
    addCommand("set-letter-color", "Help display for set letter color module.\nFormat: 'set-letter-color [HEX_COLOR]'\nSets the letter color of the shell to the specified integer.", changeLetterColor);
    addCommand("set-highlight-color", "Help display for set highlight color.\nFormat: 'set-highlight-color [HEX_COLOR]'\nSets the highlight color of the shell to the specified integer.", changehighlightColor);
    addCommand("clear", "Help display for clear module. \n Format(s): 'clear' | 'clear [LINES]'\nClears the shell or the number or shifts up the number of lines indicated.", clearTheShell);
    addCommand("dump", "Help display for dump module.\n Format: 'dump'\nDisplays a dump if one has been generated by pressing the 'alt' key. Indicates no dump has been generated otherwise.", readMeTheDump);
    addCommand("piano", "Help display for piano module.\nFormat: 'piano'\nStarts the piano module.\nPiano keys: z = Do, s = Do#, x = Re, d = Re#, c = mi, v = Fa, g = Fa#, b = Sol, h = Sol#, n = La, j = La#, m = Si", playThePiano);
    addCommand("sing", "Help display for sing module.\nFormat: 'sing [SONG_NAME]'\nSings a song. Currently recognized songs are:\n'imperial-march' 'hes-a-pirate' 'outer-wilds' 'do-i-wanna-know' 'sports-center' 'here-comes-the-sun'.", singToMe);
    addCommand("echo", "Help display for the echo module.\nFormat: 'echo [TO_ECHO]'\nIt repeats what you input.", repeat);
    addCommand("test-e", "Help display for the test exec module.\n Format: 'test-e'\nTests exec.", testExec);
}
