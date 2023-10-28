#ifndef SNK_H
#define SNK_H
#include "./../nstdlib/nstdlib.h"

#define PLAYER_COUNT 2
#define APPLE_COUNT 5
#define BOARD_WIDTH 20        // ratio is 4:3
#define BOARD_HEIGHT 15       // MUST SATISFY PLAYER_COUNT+APPLE_COUNT < (BOARD_WIDTH-2)*(BOARD_HEIGHT-2) && BOARD_WIDTH > 2 && BOARD_HEIGHT > 2. Game will 'work' anyways, but will not be particularly fun.
#define START_PLAYER_LENGTH 3 // MUST BE GREATER THAN 1 (Will work anyways, but lone heads might render as tails, don't test it)

int playSnake(uint16_t snakeCount); // return value is number of last snake to die

#endif
