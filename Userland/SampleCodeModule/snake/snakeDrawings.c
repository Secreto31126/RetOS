#include "snakeDrawings.h"
#include "snakeDrawingsPrivate.h"
#include "./../nstdlib/nstdlib.h"

#define DRAWING_COUNT 2
static snakeDrawing drawings[DRAWING_COUNT];
void setSnakeDrawings()
{
    for (int i = 0; i < DRAWING_COUNT; i++)
        drawings[i].drawings = malloc(sizeof(Drawing) * NUMBER_OF_DRAWINGS);
    drawings[0].drawSize = DRAW_SIZE;
    drawings[1].drawSize = BIG_DRAW_SIZE;

    drawings[0].drawings[HEAD_D] = (char *)classicHeadUp;
    drawings[0].drawings[BODY_D] = (char *)classicOther;
    drawings[0].drawings[TAIL_D] = (char *)classicTail;
    drawings[0].drawings[APPLE_D] = (char *)classicApple;
    // Turn not used currently
}
void freeSnakeDrawings()
{
    for (int i = 0; i < DRAWING_COUNT; i++)
        free(drawings[i].drawings);
}

snakeDrawing getCurrentDrawing()
{
    return drawings[0];
}