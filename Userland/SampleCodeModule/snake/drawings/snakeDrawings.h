#ifndef SDRAW_H
#define SDRAW_H
#include "./../../window/window.h"
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
    HexColor *growItemColorMap;
} snakeDrawing;

// cycles through currentDrawing
void makeSnakeDrawings();
void freeSnakeDrawings();
snakeDrawing getCurrentDrawing();

extern char classicHeadUp[DRAW_SIZE][DRAW_SIZE];
extern char classicOther[DRAW_SIZE][DRAW_SIZE];
extern char classicTail[DRAW_SIZE][DRAW_SIZE];
extern char classicApple[DRAW_SIZE][DRAW_SIZE];
extern char background[DRAW_SIZE][DRAW_SIZE];
extern char goomba[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern HexColor appleColorMap[];
extern HexColor marioItemColorMap[];
extern char marioItem[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern HexColor excaliburColorMap[];
extern char excalibur[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern char stone[BIG_DRAW_SIZE][BIG_DRAW_SIZE];

#endif
