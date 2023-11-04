#include "moduleSelector.h"
#include "./../shell/shell.h"
#include "./../shell/commandHandler/commandHandler.h"
#include "./../window/painter.h"
#include "./../window/window.h"
#include "./../window/fontInterface.h"
#include "./../nstdlib/nstdlib.h"
#include "./../piano/sound.h"

static const char prompt[] = "You are now in the module selector.\nPress 1 to continue to shell.\nPress 2 to get the current time.\nPress 3 to dump all registers.\nPress 4 to end program (shut down).";
void setEnvironment();

void startModules(char error)
{
    initializeFonts();
    startPainter(getScreenWidth(), getScreenHeight());
    if (error)
    {
        paintString(sPrintf("An error ocurred. Error code:\n%c\nPress any key to continue.", error), -1, 0);
        getChar();
        blank();
    }
    sports_center();
    setEnvironment();
    initializeCommands();
    char c;
    while ((c = getChar()) != '4')
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
            paintString(sPrintf("Time is %s.\nPress any key to return to module selector.", getTimeString()), -1, 0);
            freePrints();
            getChar();
            blank();
            setEnvironment();
            break;
        }
        case '3':
        {
            // Haven't gotten this far yet
            break;
        }
        default:
            break;
        }
    }
    blank();
    paintString("Exiting module selector.", -1, 0);
    wait();
    blank();
    endPainter();
    freeCommands();
}

void setEnvironment()
{
    setSize(1.0);
    paintString(prompt, -1, 0);
}