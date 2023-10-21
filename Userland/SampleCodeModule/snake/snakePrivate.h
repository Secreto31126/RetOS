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
} snake;

typedef struct tile
{
    unsigned int identifier;
    unsigned int health;
} tile;

void setBoard();

#endif
