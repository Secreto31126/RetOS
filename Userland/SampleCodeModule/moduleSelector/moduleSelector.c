#include "moduleSelector.h"
#include "./../shell/shell.h"
#include "./../shell/commandHandler/commandHandler.h"
#include "./../window/painter.h"
#include "./../window/window.h"
#include "./../window/fontInterface.h"
#include "./../nstdlib/nstdlib.h"
#include "./../piano/sound.h"

static const char prompt[] = "You are now in the module selector.\nPress 1 to continue to shell.\nPress 2 to get the current time.\nPress 3 to dump all registers.\nPress 4 to test exceptions.\nPress 5 to end program (shut down).\n";
void setEnvironment(painter p);

void startModules()
{
    painter p = startPainter(getScreenWidth(), getScreenHeight());
    // sports_center(); // por ahora no :D
    setEnvironment(p);
    commandSet comSet = initializeCommands();
    char c;
    while ((c = getChar()) != '5')
    {
        switch (c)
        {
        case '1':
        {
            blank(p);
            shellStart(p, comSet);
            setEnvironment(p);
            break;
        }
        case '2':
        {
            blank(p);
            paintString(p, sPrintf("Time is %s.\n\nPress any key to return to module selector.", getTimeString()), -1, 0);
            freePrints();
            getChar();
            blank(p);
            setEnvironment(p);
            break;
        }
        case '3':
        {
            blank(p);
            char *c = getDumpString();
            if (strcmp(c, ""))
                paintString(p, "No dump has been generated. Press 'alt' to generate a dump of the instant of pressing.\n\nPress any key to return to module selector.", -1, 0);
            else
                paintString(p, sPrintf("%s\n\nPress any key to return to module selector.", c), -1, 0);
            freePrints();
            getChar();
            blank(p);
            setEnvironment(p);
            break;
        }
        case '4':
        {
            blank(p);
            paintString(p, "You are now in the exception tests module.\nPress 1 to test the div-zero exception.\nPress 2 to test the invalid opcode exception.\nPress 3 to return to the module selector.\n", -1, 0);
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
            blank(p);
            setEnvironment(p);
        }
        default:
            break;
        }
    }
    blank(p);
    paintString(p, "Exiting module selector.", -1, 0);
    wait();
    blank(p);
    endPainter(p);
    freeCommands();
}

void setEnvironment(painter p)
{
    setSize(p, 1.0);
    paintString(p, prompt, -1, 0);
}