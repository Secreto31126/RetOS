#include "commandHandler.h"
#include "./../../nstdlib/nstdlib.h"
#include "./../../snake/drawings/backgroundArrays.h"
#include "./../../snake/drawings/snakeDrawings.h"
#include "./../../snake/snake.h"
#include "./../shell.h"
#include "commandHandlerPrivate.h"
#include <stdmem.h>
#include <sys/resource.h>
#define BLOCK 5
#define MAX_LETTER_SIZE 4
#define READ_BLOCK                                                             \
  500 // Any command parameters that exceed this will be ignored. Just don't
      // write commands exceeding 500 letters in terminal/don't feed longer than
      // BLOCK letter input to shell built-ins
#define MAX_READ_BLOCK 7999 // as much as can fit in a pipe at once
#define MAX_ARGS 32         // for execv

typedef enum routeModes { FROM_FD = 0, FROM_TERM, FROM_BOTH } routeModes;

static command *commands;
static uint64_t commandCount = 0;

char *getReadableString(moduleData source, char *buffer, int bufferSize);

void freeCommands() { free(commands); }
void addCommand(char *commandCode, char *help, action_t commandAction) {
  if (!(commandCount % BLOCK)) {
    command *aux = realloc(commands, (commandCount + BLOCK) * sizeof(command));
    if (aux == NULL)
      exit(1);
    commands = aux;
  }
  commands[commandCount].code = commandCode;
  commands[commandCount].action = commandAction;
  commands[commandCount].help = help;
  commandCount++;
}

/**
 * @brief Instantiates a component of a module
 *
 * @param command The input component in the module pairing
 * @param params Parameters to be passed to the module component being created
 * @param displayStyle Return parameter, how the module expects to be displayed
 * @return The output component in the module pairing
 */
moduleData execute(moduleData command, char *params,
                   displayStyles *displayStyle) {
  for (int i = 0; i < commandCount; i++) {
    if (isFirstWord(commands[i].code, command.s)) {
      command.s = params;
      int index;
      if ((index = isLastAlpha(params, '&')) >= 0) {
        params[index] = 0;
        moduleData toRet = commands[i].action(command, displayStyle);
        *displayStyle = AS_BACKGROUND;
        return toRet;
      }
      return commands[i].action(command, displayStyle);
    }
  }
  moduleData aux = {"Command was not recognized", -1, -1, -1};
  return aux;
}

/**
 * @brief Handles leftover file descriptors after connecting two components of a
 * module
 *
 * @param toPipe The input component in the module pairing
 * @param command The string representing the module component to be created
 * @param displayStyle Return parameter, how the module expects to be displayed
 * @return The output component in the module pairing
 */
moduleData wrapExecute(moduleData toPipe, char *command,
                       displayStyles *displayStyle) {
  moduleData aux = {command, toPipe.fd, toPipe.writeFd, toPipe.cPid};
  toPipe = execute(aux, shiftToNextWord(aux.s), displayStyle);
  if (aux.fd >= 0)
    close(aux.fd);
  if (aux.writeFd >= 0)
    close(aux.writeFd);
  return toPipe;
}

/**
 * @brief Initializes a module
 *
 * @param command The string representing the module
 * @param displayStyle Return parameter, how the module expects to be displayed
 * @param cPidBuffer Return parameter, an array of the process ids involved in
 * the creation of the module
 * @return commandData
 */
commandData handleCommand(char *command, displayStyles *displayStyle,
                          int *cPidBuffer) {

  command = shiftToWord(command);
  if (strcmpHandleWhitespace(command, "") ||
      strcmpHandleWhitespace(
          command, " ")) { // if command is empty or whitespace, it is ignored
    commandData aux = {{"", -1, -1, -1}, cPidBuffer, 0};
    return aux;
  }

  moduleData toPipe = {shiftToNextWord(command), -1, -1,
                       -1}; // The first set of params I can hand over comes
                            // from the second word in my command string
  char *currentCommandStart = command;
  int cPidCount = 0;
  for (int j = 0; command[j] && cPidCount < MAX_PIPES; j++) {
    if (command[j] == '|') {
      command[j] = 0; // Cut off the string, so I do not hand other commands
                      // over to the executor
      toPipe = wrapExecute(toPipe, currentCommandStart, displayStyle);
      currentCommandStart = shiftToWord(
          command + j +
          1); // this is also safe, as '|' is not considered whitespace and
              // would stop the shift, so j catches up
      if (toPipe.cPid >= 0)
        cPidBuffer[cPidCount++] = toPipe.cPid;
    }
  }
  toPipe = wrapExecute(toPipe, currentCommandStart, displayStyle);
  if (toPipe.cPid >= 0)
    cPidBuffer[cPidCount++] = toPipe.cPid;
  commandData toRet = {toPipe, cPidBuffer, cPidCount};
  return toRet;
}
/**
 * @brief Produces a file descriptor with the help display requested
 *
 */
moduleData getHelp(moduleData commandFd, displayStyles *displayStyle) {
  char savedSpace[READ_BLOCK];
  char *commandParameters =
      getReadableString(commandFd, savedSpace, READ_BLOCK);

  int pipeFd[2];
  moduleData toRet = {NULL, -1, -1, -1};
  if (pipe(pipeFd)) {
    toRet.s = "Could not create pipe";
    return toRet;
  }
  toRet.fd = pipeFd[READ_END];

  if (strcmpHandleWhitespace(
          "", commandParameters)) // if the command is just 'help', all help
                                  // menus are printed
  {
    char lineBreak[] = "\n\n";
    for (int i = 0; i < commandCount; i++) {
      print_sys(pipeFd[WRITE_END], commands[i].help, strlen(commands[i].help));
      print_sys(pipeFd[WRITE_END], lineBreak, sizeof(lineBreak) - 1);
    }
    close(pipeFd[WRITE_END]);
    return toRet;
  }
  for (int i = 0; i < commandCount;
       i++) // otherwise, a single matching help menu is printed if it exists.
  {
    if (isFirstWord(commandParameters, commands[i].code)) {
      print_sys(pipeFd[WRITE_END], commands[i].help,
                strlen(commands[i].help) + 1);
      close(pipeFd[WRITE_END]);
      return toRet;
    }
  }
  close(pipeFd[WRITE_END]);
  moduleData aux = {
      sPrintf("No help menu that matches '%s' was found.", commandParameters),
      -1, -1, -1}; // sPrintf automatically adds created string to list of
                   // strings to be freed by freePrints()
  return aux;
}

/**
 * @brief Initializes the snake built-in
 *
 */
moduleData startSnake(moduleData commandFd, displayStyles *displayStyle) {
  char savedSpace[READ_BLOCK];
  char *commandParameters =
      getReadableString(commandFd, savedSpace, READ_BLOCK);

  int pipeFd[2];
  moduleData toRet = {NULL, -1, -1, -1};
  if (pipe(pipeFd)) {
    toRet.s = "Could not create pipe";
    return toRet;
  }
  toRet.fd = pipeFd[READ_END];

  char *formatString = "Player %d won. Returning to shell";
  int i;
  char *aux = NULL;
  if (!strcmp("", commandParameters) || (i = atoi(commandParameters)) == 1) {
    aux = sPrintf(formatString, playSnake(1));
  }
  if (i) // if 0, either input was 0 or invalid. Either way, input is not valid
  {
    aux = sPrintf(formatString,
                  playSnake(i > 3 ? 3 : i)); // we support a third player, it is
                                             // controlled with the spacebar
  }
  if (aux != NULL) {
    *displayStyle = 1;
    print_sys(pipeFd[WRITE_END], aux, strlen(aux) + 1);
    close(pipeFd[WRITE_END]);
    return toRet;
  }
  toRet.s = "Invalid snake parameter";
  return toRet;
}

/**
 * @brief Changes the theme of the snake built-in
 *
 */
moduleData setSnakeTheme(moduleData commandFd, displayStyles *displayStyle) {
  char savedSpace[READ_BLOCK];
  char *commandParameters =
      getReadableString(commandFd, savedSpace, READ_BLOCK);

  char matchFlag = 0;
  if (!strcmp(commandParameters, "windows")) {
    setBackgroundArray((char *)windowsArray);
    setBackgroundColorMap((HexColor *)windowsColorMap);
    setSnakeDrawing(DRAW_SIZE, (char *)classicHeadUp, (char *)classicOther,
                    (char *)classicTail, (char *)classicTurn,
                    (char *)classicApple, (HexColor *)appleColorMap);
    setDrawOptions(0, 0, 1, 0);
    matchFlag = 1;
  } else if (!strcmp(commandParameters, "mario")) {
    setBackgroundArray((char *)marioArray);
    setBackgroundColorMap((HexColor *)marioColorMap);
    setSnakeDrawing(BIG_DRAW_SIZE, (char *)goomba, (char *)goomba,
                    (char *)goomba, (char *)tube, (char *)marioItem,
                    (HexColor *)marioItemColorMap);
    setDrawOptions(1, 0, 1, 1);
    matchFlag = 1;
  } else if (!strcmp(commandParameters, "creation")) {
    setBackgroundArray((char *)creationArray);
    setBackgroundColorMap((HexColor *)creationColorMap);
    setSnakeDrawing(DRAW_SIZE, (char *)classicHeadUp, (char *)classicOther,
                    (char *)classicTail, (char *)classicTurn,
                    (char *)classicApple, (HexColor *)appleColorMap);
    setDrawOptions(0, 0, 1, 0);
    matchFlag = 1;
  } else if (!strcmp(commandParameters, "idyllic")) {
    setBackgroundArray((char *)idyllicArray);
    setBackgroundColorMap((HexColor *)idyllicColorMap);
    setSnakeDrawing(BIG_DRAW_SIZE, (char *)wyvHead, (char *)wyvBody,
                    (char *)wyvTail, (char *)wyvTurn, (char *)guide,
                    (HexColor *)guideColorMap);
    setDrawOptions(1, 1, 1, 1);
    matchFlag = 1;
  }
  if (matchFlag) {
    moduleData aux = {"Theme set.", -1, -1, -1};
    return aux;
  }
  moduleData aux = {
      sPrintf("No theme matching %s was found.", commandParameters), -1, -1,
      -1};
  return aux;
}

/**
 * @brief Changes the highlight color of the shell
 *
 */
moduleData changehighlightColor(moduleData commandFd,
                                displayStyles *displayStyle) {
  char savedSpace[READ_BLOCK];
  char *commandParameters =
      getReadableString(commandFd, savedSpace, READ_BLOCK);

  if (!((*commandParameters >= '0' && *commandParameters <= '9') ||
        (*commandParameters >= 'A' && *commandParameters <= 'F') ||
        (*commandParameters >= 'a' && *commandParameters <= 'f'))) {
    moduleData aux = {"Hex value given not valid.", -1, -1, -1};
    return aux;
  }
  setHighlightColor(
      atoiHex(commandParameters)); // will read until an invalid character is
                                   // found or 8 characters have been read. If
                                   // an invalid character was found, what was
                                   // read so far will be set as the color.
  *displayStyle = 1;
  moduleData aux = {"Highlight color set", -1, -1, -1};
  return aux;
}

/**
 * @brief Changes the letter color of the shell
 *
 */
moduleData changeLetterColor(moduleData commandFd,
                             displayStyles *displayStyle) {
  char savedSpace[READ_BLOCK];
  char *commandParameters =
      getReadableString(commandFd, savedSpace, READ_BLOCK);

  if (!((*commandParameters >= '0' && *commandParameters <= '9') ||
        (*commandParameters >= 'A' && *commandParameters <= 'F') ||
        (*commandParameters >= 'a' && *commandParameters <= 'f'))) {
    moduleData aux = {"Hex value given not valid.", -1, -1, -1};
    return aux;
  }
  // uint64_t hex = 0;
  setLetterColor(atoiHex(commandParameters));
  *displayStyle = 1;
  moduleData aux = {"Letter color set", -1, -1, -1};
  return aux;
}

/**
 * @brief Changes the letter size of the shell
 *
 */
moduleData changeLetterSize(moduleData commandFd, displayStyles *displayStyle) {
  char savedSpace[READ_BLOCK];
  char *commandParameters =
      getReadableString(commandFd, savedSpace, READ_BLOCK);

  uint64_t newSize = atoi(commandParameters);
  if (newSize < 0 || newSize > MAX_LETTER_SIZE) {
    moduleData aux = {"Invalid letter size.", -1, -1, -1};
    return aux;
  }
  if (!newSize) // would be nice to actually handle doubles, however, no
                // parseDouble function can be made (reasonably easily) if SSE
                // registers cannot be used to return the value.
    resize(0.5);
  else
    resize((double)newSize);
  *displayStyle = 1;
  moduleData aux = {"Size set", -1, -1, -1};
  return aux;
}

/**
 * @brief Clears the shell
 *
 */
moduleData clearTheShell(moduleData commandFd, displayStyles *displayStyle) {
  char savedSpace[READ_BLOCK];
  char *commandParameters =
      getReadableString(commandFd, savedSpace, READ_BLOCK);

  uint64_t toClear;
  if ((toClear = atoi(commandParameters))) {
    *displayStyle = 1;
    warpNLines(toClear);
  } else
    clearShell();
  moduleData aux = {"", -1, -1, -1};
  return aux;
}

/**
 * @brief Produces a string containing a memory dump
 *
 */
moduleData readMeTheDump(moduleData commandFd, displayStyles *displayStyle) {
  char *c = getDumpString();
  if (!strcmp(c, "")) {
    moduleData aux = {"No dump generated. Press 'alt' to generate a dump of "
                      "the instant of pressing.",
                      -1, -1, -1};
    return aux;
  }
  moduleData aux = {sPrintf("The dump generated:\n%s", c), -1, -1, -1};
  return aux;
}

/**
 * @brief echo built-in
 *
 */
moduleData repeat(moduleData commandFd, displayStyles *displayStyle) {
  char savedSpace[READ_BLOCK];
  char *commandParameters =
      getReadableString(commandFd, savedSpace, READ_BLOCK);

  int pipeFd[2];
  moduleData toRet = {NULL, -1, -1, -1};
  if (pipe(pipeFd)) {
    toRet.s = "Could not create pipe";
    return toRet;
  }
  toRet.fd = pipeFd[READ_END];
  char *aux = !strcmp(commandParameters, "") ? " " : commandParameters;
  print_sys(pipeFd[WRITE_END], aux, strlen(aux) + 1);
  close(pipeFd[WRITE_END]);
  return toRet;
}

/**
 * @brief Produces a simple string from a module's output
 *
 * @param source The module to obtain the string from
 * @param buffer Where to place the string if the module communicates via file
 * descriptor
 * @param bufferSize The size of the buffer
 * @return Where the string was placed (Either the buffer or in the moduleData
 * object)
 */
char *getReadableString(moduleData source, char *buffer, int bufferSize) {
  if (source.s != NULL && *source.s)
    return source.s;
  buffer[readNFromFd(source.fd, buffer, bufferSize)] = 0;
  return buffer;
}

/**
 * @brief Executes a module, producing the specified IPC elements
 *
 * @param moduleName The name of the module to create
 * @param params The parameters to pass to the module as argv
 * @param readFd A file descriptor to dup to the module's STD_IN
 * @param routeMode How the module's file descriptors should be organized.
 * FROM_FD if it will read a file descriptor from STD_IN. FROM_TERM if it will
 * read user input from STD_IN. FROM_BOTH if it will read the former from
 * STD_IN, the latter from STD_TERM
 * @return Data from the process created. File descriptors and strings not
 * initialized are -1 and NULL respectively
 */
moduleData pipeAndExec(char *moduleName, char *params, int readFd,
                       routeModes routeMode) {
  int pipeFd[2] = {0};
  if (pipe(pipeFd)) {
    moduleData aux = {"Could not create pipe", -1, -1, -1};
    return aux;
  }

  int termPipe[2];
  if (routeMode > FROM_FD && pipe(termPipe)) {
    close(pipeFd[READ_END]);
    close(pipeFd[WRITE_END]);
    moduleData aux = {"Could not create pipe", -1, -1, -1};
    return aux;
  }

  int cPid = fork();
  if (cPid == -1) {
    close(pipeFd[READ_END]);
    close(pipeFd[WRITE_END]);
    moduleData aux = {"Could not create new process", -1, -1, -1};
    return aux;
  }
  if (!cPid) {
    // I am the child process

    setpriority(PRIO_PROC, getpid(), 5);

    // close the read end of the pipe
    close(pipeFd[READ_END]);
    // redirect stdout to the write end of the pipe
    if (dup2(pipeFd[WRITE_END], STD_OUT) < 0) {
      exit(1);
    }
    if (routeMode != FROM_TERM && readFd >= 0) {
      if (dup2(readFd, STD_IN) < 0)
        exit(1);
    } else {
      // in UNIX, closing stdin causes undefined behaviour upon attempting to
      // read stdin. A way of preventing stdin access in UNIX is to dup stdin to
      // a temporary fd, then closing that fd in RetOS, closing stdin simply
      // causes reads on stdin to return 0 without blocking, as a closed pipe
      // would.
      close(STD_IN);
    }

    if (routeMode == FROM_BOTH) {
      if (dup2(termPipe[READ_END], STD_TERM) < 0)
        exit(1);
      close(termPipe[WRITE_END]);
    } else if (routeMode == FROM_TERM) {
      if (dup2(termPipe[READ_END], STD_IN) < 0)
        exit(1);
      close(termPipe[WRITE_END]);
    }

    // whoosh
    if (params == NULL || !*params) {
      execv(moduleName, NULL);
    } else {
      char *args[MAX_ARGS];
      separateString(params, args, MAX_ARGS);
      execv(moduleName, args);
    }
    exit(1);
    moduleData aux = {"Could not execv and failed to exit, somehow.", -1, -1,
                      -1};
    return aux;
  } else if (cPid) {
    // I am the parent process
    close(pipeFd[WRITE_END]);
    if (routeMode == FROM_FD) {
      moduleData aux = {NULL, pipeFd[READ_END], -1, cPid};
      return aux;
    } else {
      close(termPipe[READ_END]);
      moduleData aux = {NULL, pipeFd[READ_END], termPipe[WRITE_END], cPid};
      return aux;
    }
  }
  moduleData aux = {"How did we get here?", -1, -1, -1};
  return aux;
}

/**
 * @brief Sends a signal to all process ids referenced directly by a module
 *
 * @param commandFd The module referencing the process ids
 * @param signal The signal to send
 * @return 1 if no process ids were referenced by the module, 0 otherwise
 */
int signalAll(moduleData commandFd, int signal) {
  char savedSpace[READ_BLOCK];
  char *args[MAX_ARGS];
  char *commandParameters =
      getReadableString(commandFd, savedSpace, READ_BLOCK);
  separateString(commandParameters, args, MAX_ARGS);

  if (args[0] == NULL) {
    return 1;
  }

  int i = 0;
  // functionality for kill KILL, kill BLOCK, kill UNBLOCK
  if (!strcmp("KILL", args[0]))
    i = 1;
  else if (!strcmp("BLOCK", args[0])) {
    signal = SIGSTOP;
    i = 1;
  } else if (!strcmp("UNBLOCK", args[0])) {
    signal = SIGCONT;
    i = 1;
  }

  for (; args[i] != NULL; i++) {
    int aux = atoi(args[i]);
    if (aux > 0) {
      kill(aux, signal);
      if (signal == SIGKILL) {
        waitpid(aux, NULL, 0);
      }
    }
  }
  return 0;
}

/**
 * @brief Sends SIGKILL to all process ids referenced
 *
 */
moduleData killer(moduleData commandFd, displayStyles *displayStyle) {

  moduleData toRet = {"Signal sent.", -1, -1, -1};
  if (signalAll(commandFd, SIGKILL))
    toRet.s = "No valid arguments read.";

  return toRet;
}

/**
 * @brief Sends SIGSTOP to all process ids referenced
 *
 */
moduleData blocker(moduleData commandFd, displayStyles *displayStyle) {
  moduleData toRet = {"Blocked.", -1, -1, -1};
  if (signalAll(commandFd, SIGSTOP))
    toRet.s = "No valid arguments read.";

  return toRet;
}

/**
 * @brief Sends SIGCONT to all process ids referenced
 *
 */
moduleData unblocker(moduleData commandFd, displayStyles *displayStyle) {

  moduleData toRet = {"Unblocked.", -1, -1, -1};
  if (signalAll(commandFd, SIGCONT))
    toRet.s = "No valid arguments read.";

  return toRet;
}

/**
 * @brief Changes the priority of a process
 *
 */
moduleData doNice(moduleData commandFd, displayStyles *displayStyle) {
  char savedSpace[READ_BLOCK];
  char *commandParameters =
      getReadableString(commandFd, savedSpace, READ_BLOCK);

  char *args[3];
  separateString(commandParameters, args, 3);

  if (args[0] == NULL || args[1] == NULL) {
    moduleData toRet = {"Not enough arguments provided.", -1, -1, -1};
    return toRet;
  }
  if (setpriority(PRIO_PROC, atoi(args[0]), atoi(args[1]))) {

    moduleData toRet = {"Could not set priority.", -1, -1, -1};
    return toRet;
  }
  moduleData toRet = {"Set.", -1, -1, -1};
  return toRet;
}

/**
 * @brief Reads out the content of a file descriptor
 *
 */
moduleData cat(moduleData commandFd, displayStyles *displayStyle) {
  // no params received, no fd to read from, use terminal as fd
  if ((!*commandFd.s || *commandFd.s == ' ') && commandFd.fd < 0)
    return pipeAndExec("cat", "TERM_MODE", commandFd.fd, FROM_TERM);
  // params/fd received, normal cat
  return pipeAndExec("cat", commandFd.s, commandFd.fd, FROM_FD);
}
/**
 * @brief Counts the number of lines in a file descriptor
 *
 */
moduleData wc(moduleData commandFd, displayStyles *displayStyle) {
  return pipeAndExec("wc", commandFd.s, commandFd.fd, FROM_FD);
}
/**
 * @brief Reads out the content of a file descriptor, vowels removed
 *
 */
moduleData filter(moduleData commandFd, displayStyles *displayStyle) {
  return pipeAndExec("filter", commandFd.s, commandFd.fd, FROM_FD);
}
/**
 * @brief Outputs a message on a set interval
 *
 */
moduleData loop(moduleData commandFd, displayStyles *displayStyle) {
  return pipeAndExec("loop", commandFd.s, commandFd.fd, FROM_FD);
}
/**
 * @brief Reads out the content of a file descriptor, keeping only the lines
 * that match a given string
 *
 */
moduleData grep(moduleData commandFd, displayStyles *displayStyle) {
  return pipeAndExec("grep", commandFd.s, commandFd.fd, FROM_FD);
}
/**
 * @brief Plays a song
 *
 */
moduleData singToMe(moduleData commandFd, displayStyles *displayStyle) {
  return pipeAndExec("sing", commandFd.s, commandFd.fd, FROM_FD);
}
/**
 * @brief Reads out the content of a file descriptor on input from the terminal
 *
 */
moduleData less(moduleData commandFd, displayStyles *displayStyle) {
  *displayStyle = NO_STDIN;
  return pipeAndExec("less", commandFd.s, commandFd.fd, FROM_BOTH);
}
/**
 * @brief Initializes the philosopher problem module
 *
 */
moduleData phylos(moduleData commandFd, displayStyles *displayStyle) {
  if (strstr(commandFd.s, "-l") != NULL)
    *displayStyle = REDRAW_ALWAYS;
  else
    *displayStyle = NO_STDIN;
  return pipeAndExec("phylos", commandFd.s, commandFd.fd, FROM_TERM);
}
/**
 * @brief Instantiates tests
 *
 */
moduleData tests(moduleData commandFd, displayStyles *displayStyle) {
  return pipeAndExec("tests", commandFd.s, commandFd.fd, FROM_FD);
}
/**
 * @brief Reports a snapshot of current processes. Note, this is a shell
 * built-in and thus not directly reflected in the snapshot
 *
 */
moduleData getPs(moduleData commandFd, displayStyles *displayStyle) {
  moduleData toRet = {NULL, ps(), -1, -1};
  if (toRet.fd < 0)
    toRet.s = "Kernel could not assign an fd to this command.";
  return toRet;
}
/**
 * @brief Reads out a report on the state of memory
 *
 */
moduleData getMem(moduleData commandFd, displayStyles *displayStyle) {
  char buffer[MAX_READ_BLOCK];
  memory_state(buffer, MAX_READ_BLOCK);
  moduleData aux = {buffer, -1, -1, -1};
  return repeat(aux, displayStyle);
}

void initializeCommands() {
  addCommand("help",
             "Help display for help module.\nFormat(s): 'help' | 'help' "
             "[MODULE_NAME]\nDisplays the help displays for all modules or the "
             "module specified.",
             getHelp);
  addCommand(
      "snake",
      "Help display for snake module.\nFormat(s): 'snake' | 'snake "
      "[PLAYERS]'\nStarts the snake module. If PLAYERS is greater than three, "
      "game will be initialized with three players. If no PLAYERS parameter is "
      "given, game will be initialized with one player. Players are controlled "
      "by 'wasd', 'ijkl', and 'v/spacebar b' key combinations.",
      startSnake);
  addCommand(
      "set-theme",
      "Help display for set theme module.\nFormat: 'set-theme [THEME]'\nSets "
      "the theme of the snake game to the specified theme.\nCurrently "
      "supported themes are:\nmario windows creation idyllic",
      setSnakeTheme);
  addCommand(
      "set-size",
      "Help display for set size module.\nFormat: 'set-size [NUMBER]'\nSets "
      "the size of the shell to the specified integer. Maximum accepted size "
      "is 4, anything larger is illegible. Only positive integer sizes are "
      "accepted. Size 0 will set the size to 0.5",
      changeLetterSize);
  addCommand("set-letter-color",
             "Help display for set letter color module.\nFormat: "
             "'set-letter-color [HEX_COLOR]'\nSets the letter color of the "
             "shell to the specified integer.",
             changeLetterColor);
  addCommand("set-highlight-color",
             "Help display for set highlight color.\nFormat: "
             "'set-highlight-color [HEX_COLOR]'\nSets the highlight color of "
             "the shell to the specified integer.",
             changehighlightColor);
  addCommand("clear",
             "Help display for clear module. \nFormat(s): 'clear' | 'clear "
             "[LINES]'\nClears the shell or the number or shifts up the number "
             "of lines indicated.",
             clearTheShell);
  addCommand("dump",
             "Help display for dump module.\nFormat: 'dump'\nDisplays a dump "
             "if one has been generated by pressing the 'alt' key. Indicates "
             "no dump has been generated otherwise.",
             readMeTheDump);
  addCommand(
      "sing",
      "Help display for sing module.\nFormat: 'sing [SONG_NAME]'\nSings a "
      "song. Currently recognized songs are:\n'imperial-march' 'hes-a-pirate' "
      "'outer-wilds' 'do-i-wanna-know' 'sports-center' 'here-comes-the-sun'.",
      singToMe);
  addCommand("echo",
             "Help display for the echo module.\nFormat: 'echo [TO_ECHO]'\nIt "
             "repeats what you input.",
             repeat);
  addCommand(
      "cat",
      "Help display for the cat module.\nFormat(s): 'cat [text] | cat "
      "[fd]'\nEchoes given text or outputs content of fd received via pipe.",
      cat);
  addCommand("wc",
             "Help display for the wc module.\nFormat: 'wc [text or fd] | cat "
             "[fd]'\nOutputs the number of lines in the given input.",
             wc);
  addCommand("filter",
             "Help display for the filter module.\nFormat: 'filter [text or "
             "fd] | cat [fd]'\nEchoes given input, vowels removed.",
             filter);
  addCommand(
      "loop",
      "Help display for the loop module.\nFormats: 'loop | loop "
      "[interval]'\nPrints its process id and a greeting on a set interval.",
      loop);
  addCommand(
      "grep",
      "Help display for the grep module.\nFormat: 'grep [match]'\nOutputs all "
      "lines from content of fd that match [match].",
      grep);
  addCommand("less",
             "Help display for the less module.\nFormat: 'less'\nOutputs "
             "content from fd upon user input.",
             less);
  addCommand("kill",
             "Help display for the kill module.\nFormat(s): 'kill [process id "
             "list] | kill [KILL | BLOCK | UNBLOCK] [process id list]'\nSends "
             "a signal to every process given, kill by default.",
             killer);
  addCommand("block",
             "Help display for the block module.\nFormat: 'block [process id "
             "list]'\nBlocks every process given.",
             blocker);
  addCommand("unblock",
             "Help display for the unblock module.\nFormat: 'unblock [process "
             "id list]'\nUnblocks every process given.",
             unblocker);
  addCommand(
      "phylos",
      "Help display for the phylos module.\nFormat: 'phylos'\nUse -l flag to "
      "enable dynamic logging.\nStarts the phylos module with 5 phylosophers, "
      "click a to add a phylosopher, r to remove one and q to quit.",
      phylos);
  addCommand("ps",
             "Help display for the ps module.\nFormat: 'ps'\nDisplays data "
             "about current running processes.",
             getPs);
  addCommand("nice",
             "Help display for the nice module.\nFormat: 'nice [process id] "
             "[new priority]'\nSets the priority of the process. Priority can "
             "range between -20 and 19, any values not in this range will be "
             "clamped to the range.\nNote: A greater priority correlates to "
             "greater time in execution.",
             doNice);
  addCommand("mem",
             "Help display for the mem module.\nFormat: 'mem'\nOutputs a "
             "report on the memory state.",
             getMem);
  addCommand("tests",
             "Help display for the tests module.\nFormat: 'tests [test name] "
             "[test parameters]' \nThis module accepts the following "
             "tests:\ntestmm [max_memory in MB]\ntestmm [max_memory in MB] "
             "[use process heap]\ntestsync [inc] [use_sem]\ntestprocesses "
             "[max_processes]\ntestprio\ntestprio [long_wait]\ntestprio "
             "[long_wait] [short_wait]",
             tests);
}
