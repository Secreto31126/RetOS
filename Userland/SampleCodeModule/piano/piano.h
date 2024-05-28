#ifndef PNO_H
#define PNO_H
#include "./sound.h"
#include "./../window/window.h"
#include "./../window/painter.h"
#include "./../shell/shell.h"
#include "./../nstdlib/nstdlib.h"

// typedef struct
// {
//     int freq;
//     char *name;
// } PianoKey;

void startPiano(painter p);
typedef struct
{
    void (*song)();
    char *name;
} Songs;

#endif
