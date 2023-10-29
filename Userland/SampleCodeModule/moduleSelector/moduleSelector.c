#include "moduleSelector.h"
#include "./../shell/shell.h"
#include "./../window/painter.h"
#include "./../window/window.h"
#include "./../window/fontInterface.h"
#include "./../nstdlib/nstdlib.h"

static const char prompt[] = "You are now in the module selector.\nPress 1 to continue to shell.\nPress 2 to get the current time.\nPress 3 to dump all registers.\nPress 4 to end program (shut down).";

void startModules()
{
    initializeFonts();
    startPainter(getScreenWidth(), getScreenHeight());
    setSize(1.0);
    paintString(prompt, -1, 0);

    char c;
    while ((c = getChar()) != '4')
    {
        switch (c)
        {
        case '1':
        {
            blank();
            shellStart();
            paintString(prompt, -1, 0);
            break;
        }
        case '2':
        {
            blank();
            char *toPrint = sPrintf("Time is %d:%d.\nPress any key to return to module selector.", getHours(), getMinutes());
            paintString(toPrint, -1, 0);
            free(toPrint);
            char aux[10];                                  // remove
            paintString(itoa(getHours(), aux, 10), -1, 0); // remove
            paintChar(':', -1, 0);
            paintString(itoa(getMinutes(), aux, 10), -1, 0); // remove
            getChar();
            blank();
            paintString(prompt, -1, 0);
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
}