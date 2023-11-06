#ifndef DRWM_H
#define DRWM_H

#include "./../../window/window.h"
#include <stdint.h>
#include <stdarg.h>

char *fillCharFromParams(uint64_t paramCount, ...);
HexColor *fillHexFromParams(uint64_t paramCount, ...);
void initializeDrawings();
void freeDrawings();
HexColor colorMapper(HexColor *colorMap, int code);

// backgrounds
#define BACKGROUND_WIDTH 102
#define BACKGROUND_HEIGHT 77
extern HexColor *windowsColorMap;
extern char *windowsArray;
extern HexColor *marioColorMap;
extern char *marioArray;
extern HexColor *pongColorMap;
extern char *pongArray;
extern HexColor *creationColorMap;
extern char *creationArray;
extern HexColor *camelotColorMap;
extern char *camelotArray;
HexColor colorMapper(HexColor *colorMap, int code);
/*
// drawings
#define DRAW_SIZE 12
#define BIG_DRAW_SIZE 24
#define TEXT_BOARD_WIDTH 36
#define TEXT_BOARD_HEIGHT 24
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
extern char pipe[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern HexColor excaliburColorMap[];
extern char excalibur[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern char stone[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern char catapult[BIG_DRAW_SIZE][BIG_DRAW_SIZE];
extern char textBoard[TEXT_BOARD_HEIGHT][TEXT_BOARD_WIDTH];
extern HexColor textBoardColorMap[];
*/
#endif
