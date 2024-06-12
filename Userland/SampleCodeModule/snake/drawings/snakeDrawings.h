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

extern const char classicHeadUp[DRAW_SIZE][DRAW_SIZE];
extern const char classicOther[DRAW_SIZE][DRAW_SIZE];
extern const char classicTurn[DRAW_SIZE][DRAW_SIZE];
extern const char classicTail[DRAW_SIZE][DRAW_SIZE];
extern const char classicApple[DRAW_SIZE][DRAW_SIZE];
extern const char background[DRAW_SIZE][DRAW_SIZE];
extern const char goomba[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern const HexColor appleColorMap[];
extern const HexColor marioItemColorMap[];
extern const char marioItem[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern const char tube[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern const HexColor excaliburColorMap[];
extern const char excalibur[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern const char stone[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern const char catapult[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern const char textBoard[TEXT_BOARD_HEIGHT][TEXT_BOARD_WIDTH];
extern const HexColor textBoardColorMap[];
extern const char wyvBody[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern const char wyvHead[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern const char wyvTail[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern const char wyvTurn[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern const char guide[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern const HexColor guideColorMap[];
extern const char demonHead[DRAW_SIZE][DRAW_SIZE];
extern const char demonOther[DRAW_SIZE][DRAW_SIZE];
extern const char demonTurn[DRAW_SIZE][DRAW_SIZE];
extern const char demonTail[DRAW_SIZE][DRAW_SIZE];
extern const char doomGuy[DRAW_SIZE][DRAW_SIZE];
extern const HexColor doomGuyColorMap[];

#endif
