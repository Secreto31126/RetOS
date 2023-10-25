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
typedef enum TO_DRAW
{
    NO_DRAW,
    BLANK,
    HEAD,
    BODY,
    TAIL,
    SHEEP // apple was taken
} TO_DRAW;

typedef struct snake
{
    unsigned int length;
    DIRECTION direction;
    uint64_t color;
    char alive;
    char moved;
} snake;

typedef struct tile
{
    char identifier;
    unsigned int health;
    TO_DRAW toDraw;
} tile;

void setBoard();

#endif
