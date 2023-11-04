#ifndef SNKP_H
#define SNKP_H

#define BACKGROUND_COLOR 0x00000000

#include "./../window/fontInterface.h"
#include "./../window/window.h"
#include "./../window/painter.h"
// #include "./../window/figures.h" // no longer used
#include "random.h"

typedef enum DIRECTION
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NONE,
} DIRECTION;
typedef enum TO_DRAW
{
    NO_DRAW,
    BLANK,
    HEAD,
    BODY,
    TAIL,
    TURN,
    APPLE, // apple was taken
} TO_DRAW;

typedef struct
{
    unsigned int length;
    DIRECTION direction;
    uint64_t color;
    char alive;
    char nextHeadCoordinates[2];
} snake;

typedef struct tile
{
    char player;
    unsigned int health;
    TO_DRAW toDraw;
    DIRECTION trueDirection;
    DIRECTION drawDirection;
} tile;

void setBoard(int snakeCount);
void setDirection(unsigned int playerNumber, DIRECTION direction);
tile *getBoard();
snake *getSnakes();
unsigned int update();
void freeBack();

#endif
