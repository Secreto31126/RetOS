#ifndef SDRAW_H
#define SDRAW_H

#define DRAW_SIZE 12
#define BIG_DRAW_SIZE 24
typedef struct snakeDrawing
{
    int drawSize;
    char **drawings;
} snakeDrawing;

typedef enum Drawing
{
    HEAD_D = 0,
    BODY_D,
    TAIL_D,
    TURN_D,
    APPLE_D,
    NUMBER_OF_DRAWINGS
} Drawing;

// cycles through currentDrawing
void setSnakeDrawings();
void cycleDrawing();
void freeSnakeDrawings();
snakeDrawing getCurrentDrawing();

#endif
