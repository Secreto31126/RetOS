#ifndef SDRAW_H
#define SDRAW_H

#define DRAW_SIZE 12
#define BIG_DRAW_SIZE 24
typedef struct snakeDrawing
{
    int drawSize;
    char *headDrawing;
    char *bodyDrawing;
    char *tailDrawing;
    char *turnDrawing;
    char *growItemDrawing;
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
void makeSnakeDrawings();
void freeSnakeDrawings();
snakeDrawing getCurrentDrawing();

extern char classicHeadUp[DRAW_SIZE][DRAW_SIZE];
extern char classicOther[DRAW_SIZE][DRAW_SIZE];
extern char classicTail[DRAW_SIZE][DRAW_SIZE];
extern char classicApple[DRAW_SIZE][DRAW_SIZE];
extern char background[DRAW_SIZE][DRAW_SIZE];

#endif
