#ifndef SNKP_H
#define SNKP_H

typedef enum DIRECTION
{
    NONE,
    LEFT,
    RIGHT,
    UP,
    DOWN
} DIRECTION;

typedef struct snake
{
    unsigned int length;
    DIRECTION direction;
    uint64_t color;
    char alive;
    char growing;
} snake;

typedef struct tile
{
    char identifier;
    unsigned int health;
} tile;

void setBoard();

#endif
