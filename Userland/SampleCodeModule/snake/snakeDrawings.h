#ifndef SNKD_H
#define SNKD_H
#define DRAW_SIZE 12
typedef struct snakeDrawing
{
    char *headUp;
    char *headDown;
    char *headLeft;
    char *headRight;
    char *body;
    char *tail;
    char *bLTurn;
    char *bRTurn;
    char *tLTurn;
    char *tRTurn;
} snakeDrawing;

char classicHeadUp[DRAW_SIZE][DRAW_SIZE] = {
    {0},
    {0},
    {0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0}};
char classicHeadDown[DRAW_SIZE][DRAW_SIZE] = {
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0},
    {0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
    {0},
    {0},
    {0}};
char classicHeadRight[DRAW_SIZE][DRAW_SIZE] = {
    {0},
    {0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0},
    {0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0},
    {0}};
char classicHeadLeft[DRAW_SIZE][DRAW_SIZE] = {
    {0},
    {0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0},
    {0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0},
    {0}};
char classicOther[DRAW_SIZE][DRAW_SIZE] = {{0}};

#endif
