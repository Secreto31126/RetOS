#ifndef SDRAW_H
#define SDRAW_H
#define DRAW_SIZE 12
#define BIG_DRAW_SIZE 24
#define TEXT_BOARD_WIDTH 36
#define TEXT_BOARD_HEIGHT 24
#include "./../../window/window.h"
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
extern char classicTurn[DRAW_SIZE][DRAW_SIZE];
extern char classicTail[DRAW_SIZE][DRAW_SIZE];
extern char classicApple[DRAW_SIZE][DRAW_SIZE];
extern char background[DRAW_SIZE][DRAW_SIZE];
extern char goomba[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern HexColor appleColorMap[];
extern HexColor marioItemColorMap[];
extern char marioItem[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern char tube[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern HexColor excaliburColorMap[];
extern char excalibur[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern char stone[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern char catapult[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern char textBoard[TEXT_BOARD_HEIGHT][TEXT_BOARD_WIDTH];
extern HexColor textBoardColorMap[];
extern char wyvBody[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern char wyvHead[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern char wyvTail[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern char wyvTurn[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern char guide[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern HexColor guideColorMap[];
extern char demonHead[DRAW_SIZE][DRAW_SIZE];
extern char demonOther[DRAW_SIZE][DRAW_SIZE];
extern char demonTurn[DRAW_SIZE][DRAW_SIZE];
extern char demonTail[DRAW_SIZE][DRAW_SIZE];
extern char doomGuy[DRAW_SIZE][DRAW_SIZE];
extern HexColor doomGuyColorMap[];

#endif
