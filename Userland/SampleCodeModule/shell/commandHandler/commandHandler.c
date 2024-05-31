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
#define READ_BLOCK 500 // Any command parameters that exceed this will be ignored. Just don't write commands exceeding 50 letters in terminal/don't feed longer than BLOCK letter input to shell built-ins
#define MAX_ARGS 255   // for execv

static command *commands;
static uint64_t commandCount = 0;

char *getReadableString(stringOrFd source, char *buffer, int bufferSize);

void freeCommands()
{
    free(commands);
}
void addCommand(char *commandCode, char *help, action_t commandAction)
{
    if (!(commandCount % BLOCK))
        commands = realloc(commands, commandCount * sizeof(command), (commandCount + BLOCK) * sizeof(command));
    commands[commandCount].code = commandCode;
    commands[commandCount].action = commandAction;
    commands[commandCount].help = help;
    commandCount++;
}

stringOrFd execute(stringOrFd command, char *params, displayStyles *displayStyle)
{
    for (int i = 0; i < commandCount; i++)
    {
        if (isFirstWord(commands[i].code, command.s))
        {
            command.s = params;
            return commands[i].action(command, displayStyle);
        }
    }
    stringOrFd aux = {"Command was not recognized", -1};
    return aux;
}

stringOrFd wrapExecute(stringOrFd toPipe, char *command, displayStyles *displayStyle)
{
    if (toPipe.s != NULL)
    {
        stringOrFd aux = {command, -1};
        return execute(aux, shiftToNextWord(aux.s), displayStyle);
    }
    else
    { // I have an open read fd, I hand it over to the executor, and close it once it is no longer in use
        stringOrFd aux = {command, toPipe.fd};
        toPipe = execute(aux, shiftToNextWord(aux.s), displayStyle);
        close(aux.fd);
        return toPipe;
    }
}

stringOrFd handleCommand(char *command, displayStyles *displayStyle)
{

    command = shiftToWord(command);
    if (strcmpHandleWhitespace(command, "") || strcmpHandleWhitespace(command, " "))
    { // if command is empty or whitespace, it is ignored
        stringOrFd aux = {"", -1};
        return aux;
    }

    stringOrFd toPipe = {shiftToNextWord(command), -1}; // The first set of params I can hand over comes from the second word in my command string
    char *currentCommandStart = command;
    for (int j = 0; command[j]; j++)
    {
        if (command[j] == '|')
        {
            command[j] = 0; // Cut off the string, so I do not hand other commands over to the executor
            toPipe = wrapExecute(toPipe, currentCommandStart, displayStyle);
            currentCommandStart = shiftToWord(command + j + 1); // this is also safe, as '|' is not considered whitespace and would stop the shift, so j catches up
        }
    }
    return wrapExecute(toPipe, currentCommandStart, displayStyle);
}

stringOrFd getHelp(stringOrFd commandFd, displayStyles *displayStyle)
{
    char savedSpace[READ_BLOCK];
    char *commandParameters = getReadableString(commandFd, savedSpace, READ_BLOCK);

    int pipeFd[2];
    stringOrFd toRet = {NULL, -1};
    if (pipe(pipeFd))
    {
        toRet.s = "Could not create pipe";
        return toRet;
    }
    toRet.fd = pipeFd[READ_END];

    if (strcmpHandleWhitespace("", commandParameters)) // if the command is just 'help', all help menus are printed
    {
        char lineBreak[] = "\n\n";
        for (int i = 0; i < commandCount; i++)
        {
            print_sys(pipeFd[WRITE_END], commands[i].help, strlen(commands[i].help));
            print_sys(pipeFd[WRITE_END], lineBreak, sizeof(lineBreak) - 1);
        }
        close(pipeFd[WRITE_END]);
        return toRet;
    }
    for (int i = 0; i < commandCount; i++) // otherwise, a single matching help menu is printed if it exists.
    {
        if (isFirstWord(commandParameters, commands[i].code))
        {
            print_sys(pipeFd[WRITE_END], commands[i].help, strlen(commands[i].help) + 1);
            close(pipeFd[WRITE_END]);
            return toRet;
        }
    }
    close(pipeFd[WRITE_END]);
    stringOrFd aux = {sPrintf("No help menu that matches '%s' was found.", commandParameters), -1}; // sPrintf automatically adds created string to list of strings to be freed by freePrints()
    return aux;
}

stringOrFd startSnake(stringOrFd commandFd, displayStyles *displayStyle)
{
    char savedSpace[READ_BLOCK];
    char *commandParameters = getReadableString(commandFd, savedSpace, READ_BLOCK);

    int pipeFd[2];
    stringOrFd toRet = {NULL, -1};
    if (pipe(pipeFd))
    {
        toRet.s = "Could not create pipe";
        return toRet;
    }
    toRet.fd = pipeFd[READ_END];

    char *formatString = "Player %d won. Returning to shell";
    int i;
    char *aux = NULL;
    if (strcmp("", commandParameters) || (i = atoi(commandParameters)) == 1)
    {
        aux = sPrintf(formatString, playSnake(1));
    }
    if (i) // if 0, either input was 0 or invalid. Either way, input is not valid
    {
        aux = sPrintf(formatString, playSnake(i > 3 ? 3 : i)); // we support a third player, it is controlled with the spacebar
    }
    if (aux != NULL)
    {
        *displayStyle = 1;
        print_sys(pipeFd[WRITE_END], aux, strlen(aux) + 1);
        close(pipeFd[WRITE_END]);
        return toRet;
    }
    toRet.s = "Invalid snake parameter";
    return toRet;
}

stringOrFd setSnakeTheme(stringOrFd commandFd, displayStyles *displayStyle)
{
    char savedSpace[READ_BLOCK];
    char *commandParameters = getReadableString(commandFd, savedSpace, READ_BLOCK);

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
        stringOrFd aux = {"Theme set.", -1};
        return aux;
    }
    stringOrFd aux = {sPrintf("No theme matching %s was found.", commandParameters), -1};
    return aux;
}

stringOrFd changehighlightColor(stringOrFd commandFd, displayStyles *displayStyle)
{
    char savedSpace[READ_BLOCK];
    char *commandParameters = getReadableString(commandFd, savedSpace, READ_BLOCK);

    if (!((*commandParameters >= '0' && *commandParameters <= '9') || (*commandParameters >= 'A' && *commandParameters <= 'F') || (*commandParameters >= 'a' && *commandParameters <= 'f')))
    {
        stringOrFd aux = {"Hex value given not valid.", -1};
        return aux;
    }
    setHighlightColor(atoiHex(commandParameters)); // will read until an invalid character is found or 8 characters have been read. If an invalid character was found, what was read so far will be set as the color.
    *displayStyle = 1;
    stringOrFd aux = {"Highlight color set", -1};
    return aux;
}
stringOrFd changeLetterColor(stringOrFd commandFd, displayStyles *displayStyle)
{
    char savedSpace[READ_BLOCK];
    char *commandParameters = getReadableString(commandFd, savedSpace, READ_BLOCK);

    if (!((*commandParameters >= '0' && *commandParameters <= '9') || (*commandParameters >= 'A' && *commandParameters <= 'F') || (*commandParameters >= 'a' && *commandParameters <= 'f')))
    {
        stringOrFd aux = {"Hex value given not valid.", -1};
        return aux;
    }
    // uint64_t hex = 0;
    setLetterColor(atoiHex(commandParameters));
    *displayStyle = 1;
    stringOrFd aux = {"Letter color set", -1};
    return aux;
}
stringOrFd changeLetterSize(stringOrFd commandFd, displayStyles *displayStyle)
{
    char savedSpace[READ_BLOCK];
    char *commandParameters = getReadableString(commandFd, savedSpace, READ_BLOCK);

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
    *displayStyle = 1;
    stringOrFd aux = {"Size set", -1};
    return aux;
}
stringOrFd clearTheShell(stringOrFd commandFd, displayStyles *displayStyle)
{
    char savedSpace[READ_BLOCK];
    char *commandParameters = getReadableString(commandFd, savedSpace, READ_BLOCK);

    uint64_t toClear;
    if ((toClear = atoi(commandParameters)))
    {
        *displayStyle = 1;
        warpNLines(toClear);
    }
    else
        clearShell();
    stringOrFd aux = {"", -1};
    return aux;
}
stringOrFd readMeTheDump(stringOrFd commandFd, displayStyles *displayStyle)
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
stringOrFd playThePiano(stringOrFd commandFd, displayStyles *displayStyle)
{
    *displayStyle = 1;
    startPiano();
    stringOrFd aux = {"Now exiting the yellow submarine.", -1};
    return aux;
}
stringOrFd singToMe(stringOrFd commandFd, displayStyles *displayStyle)
{
    char savedSpace[READ_BLOCK];
    char *commandParameters = getReadableString(commandFd, savedSpace, READ_BLOCK);

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
stringOrFd repeat(stringOrFd commandFd, displayStyles *displayStyle)
{
    char savedSpace[READ_BLOCK];
    char *commandParameters = getReadableString(commandFd, savedSpace, READ_BLOCK);

    int pipeFd[2];
    stringOrFd toRet = {NULL, -1};
    if (pipe(pipeFd))
    {
        toRet.s = "Could not create pipe";
        return toRet;
    }
    toRet.fd = pipeFd[READ_END];
    char *aux = strcmp(commandParameters, "") ? " " : commandParameters;
    print_sys(pipeFd[WRITE_END], aux, strlen(aux) + 1);
    close(pipeFd[WRITE_END]);
    return toRet;
}

char *getReadableString(stringOrFd source, char *buffer, int bufferSize)
{
    if (source.s != NULL && *source.s)
        return source.s;
    buffer[readNFromFd(source.fd, buffer, bufferSize)] = 0;
    return buffer;
}

stringOrFd pipeAndExec(char *moduleName, char *params, int readFd)
{
    int pipeFd[2] = {0};
    if (pipe(pipeFd))
    {
        stringOrFd aux = {"Could not create pipe", -1};
        return aux;
    }
    int c_pid = fork();
    if (c_pid == -1)
    {
        stringOrFd aux = {"Could not create new process", -1};
        return aux;
    }
    if (!c_pid)
    {
        // I am the child process

        // close the read end of the pipe
        close(pipeFd[READ_END]);
        // redirect stdout to the write end of the pipe
        if (dup2(pipeFd[WRITE_END], 1) < 0)
        {
            stringOrFd aux = {"Could not dup2", -1};
            return aux;
        }
        if (readFd >= 0)
        {
            if (dup2(readFd, 0) < 0)
            {
                stringOrFd aux = {"Could not dup2", -1};
                return aux;
            }
        }
        else
        {
            // in UNIX, closing stdin causes undefined behaviour upon attempting to read stdin. A way of preventing stdin access in UNIX is to dup stdin to a temporary fd, then closing that fd
            // in RetOS, closing stdin simply causes reads on stdin to return 0 without blocking, as a closed pipe would.
            close(STD_IN);
        }

        // whoosh
        if (params == NULL || !*params)
        {
            execv(moduleName, NULL);
        }
        else
        {
            char *args[MAX_ARGS];
            separateString(params, args, MAX_ARGS);
            execv(moduleName, args);
        }

        stringOrFd aux = {"Could not execv", -1};
        return aux;
    }
    else if (c_pid)
    {
        // I am the parent process
        close(pipeFd[WRITE_END]);
        stringOrFd aux = {NULL, pipeFd[READ_END]};
        return aux;
    }
    stringOrFd aux = {"How did we get here?", -1};
    return aux;
}

stringOrFd cat(stringOrFd commandFd, displayStyles *displayStyle)
{
    return pipeAndExec("cat", commandFd.s, commandFd.fd);
}
stringOrFd wc(stringOrFd commandFd, displayStyles *displayStyle)
{
    return pipeAndExec("wc", commandFd.s, commandFd.fd);
}
stringOrFd filter(stringOrFd commandFd, displayStyles *displayStyle)
{
    return pipeAndExec("filter", commandFd.s, commandFd.fd);
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
    addCommand("cat", "Help display for the cat module.\n Format(s): 'cat [text] | cat [fd]'\nEchoes given text or outputs content of fd received via pipe.", cat);
    addCommand("wc", "Help display for the wc module.\n Format: 'wc [text or fd] | cat [fd]'\nOutputs the number of lines in the given input.", wc);
    addCommand("filter", "Help display for the filter module.\n Format: 'filter [text or fd] | cat [fd]'\nEchoes given input, vowels removed.", filter);
}
