#include "moduleSelector.h"
#include "./../shell/shell.h"
#include "./../shell/commandHandler/commandHandler.h"
#include "./../window/painter.h"
#include "./../window/window.h"
#include "./../window/fontInterface.h"
#include "./../nstdlib/nstdlib.h"
#include "./../piano/sound.h"
#include <sys/resource.h>

static const char prompt[] = "You are now in the module selector.\nPress 1 to continue to shell.\nPress 2 to get the current time.\nPress 3 to dump all registers.\nPress 4 to test exceptions.\nPress 5 to end program (shut down).\n";
void setEnvironment();

void startModules()
{
    setpriority(PRIO_PROCESS, getpid(), -20);
    startPainter(getScreenWidth(), getScreenHeight());
    // sports_center(); // por ahora no :D
    setEnvironment();
    initializeCommands();
    char c;
    while ((c = getChar()) != '5')
    {
        switch (c)
        {
        case '1':
        {
            blank();
            shellStart();
            setEnvironment();
            break;
        }
        case '2':
        {
            blank();
            char buffer[6];
            paintString(sPrintf("Time is %s.\n\nPress any key to return to module selector.", getTimeString(buffer)), -1, 0);
            freePrints();
            getChar();
            blank();
            setEnvironment();
            break;
        }
        case '3':
        {
            blank();
            char *c = getDumpString();
            if (!strcmp(c, ""))
                paintString("No dump has been generated. Press 'alt' to generate a dump of the instant of pressing.\n\nPress any key to return to module selector.", -1, 0);
            else
                paintString(sPrintf("%s\n\nPress any key to return to module selector.", c), -1, 0);
            freePrints();
            getChar();
            blank();
            setEnvironment();
            break;
        }
        case '4':
        {
            blank();
            paintString("You are now in the exception tests module.\nPress 1 to test the div-zero exception.\nPress 2 to test the invalid opcode exception.\nPress 3 to return to the module selector.\n", -1, 0);
            while ((c = getChar()) != '3')
            {
                int i = 1;
                if (c == '1')
                    i = i / (i - 1);
                if (c == '2')
                {
                    char code[] = "ඞඞඞ";
                    ((void (*)(void))code)();
                }
            }
            blank();
            setEnvironment();
        }
        default:
            break;
        }
    }
    blank();
    paintString("Exiting module selector.", -1, 0);
    sleep(1);
    blank();
    endPainter();
    freeCommands();
}

void setEnvironment()
{
    setSize(1.0);
    paintString(prompt, -1, 0);
}