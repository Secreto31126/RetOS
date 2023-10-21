#ifndef SNK_H
#define SNK_H
#include "./../nstdlib/nstdlib.h"

#define PLAYER_COUNT 2
#define APPLE_COUNT 1
#define BOARD_WIDTH 40        // ratio is 4:3
#define BOARD_HEIGHT 30       // MUST SATISFY PLAYER_COUNT+APPLE_COUNT < (BOARD_WIDTH-2)*(BOARD_HEIGHT-2) && BOARD_WIDTH > 2 && BOARD_HEIGHT > 2 && BOARD_WIDTH*BOARD_HEIGHT*(PLAYER_COUNT+1) < maxNumber(tile). Game will 'work' anyways, but will not be particularly fun.
#define START_PLAYER_LENGTH 2 // MUST BE GREATER THAN 1 (Will work anyways, but lone heads might render as tails, don't test it)
typedef unsigned int tile;

int snake(); // return value is number of last snake to die

#endif
