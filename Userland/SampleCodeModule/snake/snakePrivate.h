#ifndef SNKP_H
#define SNKP_H
#include "./../window/fontInterface.h"
#include "./../window/window.h"
#include "./../window/painter.h"
#include "random.h"
#include "snakeDrawings.h"

typedef enum DIRECTION
{
    NONE,
    LEFT,
    RIGHT,
    UP,
    DOWN,
} DIRECTION;
typedef enum TO_DRAW
{
    NO_DRAW,
    BLANK,
    HEAD,
    BODY,
    TAIL,
    B_L_TURN,
    B_R_TURN,
    T_L_TURN,
    T_R_TURN,
    APPLE, // apple was taken
} TO_DRAW;

typedef struct
{
    unsigned int length;
    DIRECTION direction;
    uint64_t color;
    char alive;
    char nextHeadCoordinates[2];
    DIRECTION lastMove; // Currently unused, for drawing more complex snakes
} snake;

typedef struct tile
{
    char player;
    unsigned int health;
    TO_DRAW toDraw;
} tile;

void setBoard(int snakeCount);
void setDirection(unsigned int playerNumber, DIRECTION direction);
tile *getBoard();
snake *getSnakes();
unsigned int update();
extern uint64_t get_tick();

#endif
