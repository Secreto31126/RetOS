#ifndef SNK_H
#define SNK_H
#include "./../nstdlib/nstdlib.h"
#include "./../window/window.h"

#define PLAYER_COUNT 2
#define APPLE_COUNT 5
#define BOARD_WIDTH 20        // ratio is 4:3
#define BOARD_HEIGHT 15       // MUST SATISFY PLAYER_COUNT+APPLE_COUNT < (BOARD_WIDTH-2)*(BOARD_HEIGHT-2) && BOARD_WIDTH > 2 && BOARD_HEIGHT > 2. Game will 'work' anyways, but will not be particularly fun.
#define START_PLAYER_LENGTH 3 // MUST BE GREATER THAN 1 (Will work anyways, but lone heads might render as tails, don't test it)
#define MOVE_INTERVAL 2
#define MAX_SNAKE_COLORS 5

int playSnake(uint16_t snakeCount); // return value is number of last snake to die
void setTheme();
void setGrowItemColorMap(HexColor *newBackground);
void setBackgroundColorMap(HexColor *newBackground);
void setBackgroundArray(char *newBackground);
void setSnakeDrawing(char drawSize, char *headDrawing, char *bodyDrawing, char *tailDrawing, char *turnDrawing, char *growItemDrawing, HexColor *growItemColorMap);
void setDrawOptions(char headRedraw, char bodyRedraw, char tailRedraw, char turnRedraw);

#endif
