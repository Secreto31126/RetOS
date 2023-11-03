#include "snake.h"
#include "snakePrivate.h"
#include "random.h"
#include "./../piano/sound.h"

#define EMPTY 0
#define BOARD_SIZE (BOARD_HEIGHT * BOARD_WIDTH)
#define BOARD_SIZE_NO_MARGINS ((BOARD_HEIGHT - 2) * (BOARD_WIDTH - 2))

static tile board[BOARD_HEIGHT][BOARD_WIDTH];
static snake *snakes;
static unsigned int objects;

unsigned int getNthEmpty(unsigned int n);
char isTail(tile t);
char isHead(tile t);
signed int parseDirX(DIRECTION dir);
signed int parseDirY(DIRECTION dir);
void killSnake(unsigned int player);
void growSnake(unsigned int player);
void makeApple();
void setNewHeads(int snakeCount);

void setBoard(int snakeCount)
{
    snakes = malloc(sizeof(snake) * snakeCount);
    objects = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        board[0][i].health = 0;
        board[0][i].player = 0;
        board[0][i].toDraw = NO_DRAW;
        board[0][i].drawDirection = UP;
    }

    uint64_t boardSizeNoMargins = BOARD_SIZE_NO_MARGINS; // just to avoid calculating it in every loop
    for (int i = 0; i < APPLE_COUNT; i++)
    {
        unsigned int j = getNthEmpty(randBetween(0, boardSizeNoMargins - i)); // no objects will be placed on edges upon setting board.
        if (j < boardSizeNoMargins)
        { // No objects will be added if board is full.
            board[0][j].toDraw = APPLE;
        }
    }
    for (int i = 0; i < snakeCount; i++)
    {
        unsigned int j = getNthEmpty(randBetween(0, boardSizeNoMargins - i - APPLE_COUNT)); // no objects will be placed on edges on setting board.
        if (j < boardSizeNoMargins)
        {
            board[0][j].player = i; // No objects will be added if board is full.
            board[0][j].health = START_PLAYER_LENGTH;
            board[0][j].toDraw = HEAD;
        }
        snakes[i].length = START_PLAYER_LENGTH;
        snakes[i].color = getHexColor();
        snakes[i].direction = ((j % BOARD_WIDTH) > (BOARD_WIDTH / 2)) ? LEFT : RIGHT; // This way you don't start moving towards the edge. Still won't spawn at the edge out of respect.
        snakes[i].alive = 1;
    }
    objects = APPLE_COUNT + snakeCount * START_PLAYER_LENGTH; // counts non-empty tiles, useful for resetting apples
}

void setDirection(unsigned int playerNumber, DIRECTION direction)
{
    DIRECTION prev = snakes[playerNumber].direction;
    if (!(prev == UP && direction == DOWN) && !(prev == DOWN && direction == UP) && !(prev == LEFT && direction == RIGHT) && !(prev == RIGHT && direction == LEFT) && direction != NONE)
        snakes[playerNumber].direction = direction;
}

// returns 0 if no players died. Returns the player player if a player died.
unsigned int update(int snakeCount)
{
    char toReturn = 0;
    tile lookingAt;
    for (int i = 0; i < BOARD_HEIGHT; i++)
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            lookingAt = board[i][j];
            if (lookingAt.health > 0) // All else should have health 0
            {
                if (lookingAt.toDraw == BLANK) // allows blanks to remain for one reading to ensure they are not turned to NO_DRAW before they can be read by the front-end
                {
                    board[i][j].health = 0;
                }
                else if (isHead(lookingAt))
                {
                    int nextX = j + parseDirX(snakes[lookingAt.player].direction);
                    int nextY = i + parseDirY(snakes[lookingAt.player].direction);
                    if (nextX < 0 || nextX >= BOARD_WIDTH || nextY < 0 || nextY >= BOARD_HEIGHT || (board[nextY][nextX].health != 0 && board[nextX][nextY].toDraw != BLANK && board[nextX][nextY].toDraw != APPLE))
                    {
                        killSnake(lookingAt.player);
                        toReturn = lookingAt.player + 1;
                    }
                    else
                    {
                        if (board[nextY][nextX].toDraw == APPLE)
                        {
                            play(390);
                            growSnake(lookingAt.player);
                            makeApple();
                            shut();
                        }
                        snakes[lookingAt.player].nextHeadCoordinates[0] = nextX;
                        snakes[lookingAt.player].nextHeadCoordinates[1] = nextY;
                        board[i][j].toDraw = BODY;
                        board[i][j].drawDirection = snakes[lookingAt.player].direction;
                    }
                }
                else
                {
                    board[i][j].toDraw = NO_DRAW; // No need to update unmoving parts until they die.
                }
                board[i][j].health--; // All snake parts lose one 'health' per movement. This way, parts remain for as many movements as the snake is long, giving the appearance of a continuous snake. Using players to uniformly color snakes reinforces this
                if (board[i][j].health == 1)
                    board[i][j].toDraw = TAIL;
                if (board[i][j].health == 0)
                    board[i][j].toDraw = BLANK;
            }
            else if (lookingAt.toDraw != APPLE) // apples redrawn at every turn currently. Prevents not drawing new apples
            {
                board[i][j].toDraw = NO_DRAW;
            }
        }
    setNewHeads(snakeCount);
    return toReturn; // player of dead snake, or 0
}
void freeBack()
{
    free(snakes);
}

void setNewHeads(int snakeCount)
{
    for (int i = 0; i < snakeCount; i++)
    {
        snake s = snakes[i];
        if (s.alive)
        {
            board[s.nextHeadCoordinates[1]][s.nextHeadCoordinates[0]].player = i;
            board[s.nextHeadCoordinates[1]][s.nextHeadCoordinates[0]].toDraw = HEAD;
            board[s.nextHeadCoordinates[1]][s.nextHeadCoordinates[0]].health = s.length;
            board[s.nextHeadCoordinates[1]][s.nextHeadCoordinates[0]].drawDirection = s.direction;
        }
    }
}

void killSnake(unsigned int player)
{
    for (int i = 0; i < BOARD_SIZE; i++)
        if (board[0][i].player == player && board[0][i].health != 0)
        {
            board[0][i].health = 1; // Tiles with health 1 and toDraw BLANK will be drawn as background and become NO_DRAW on next update loop
            board[0][i].toDraw = BLANK;
        }
    snakes[player].alive = 0;
}
void growSnake(unsigned int player)
{
    for (int i = 0; i < BOARD_SIZE; i++)
        if (board[0][i].player == player && board[0][i].health != 0)
            board[0][i].health++;
    snakes[player].length++;
    objects++;
}
void makeApple()
{
    uint64_t boardSizeNoMargins = BOARD_SIZE_NO_MARGINS;
    unsigned int j = getNthEmpty(randBetween(0, boardSizeNoMargins - objects)); // no objects will be placed on edges on setting board.
    if (j < boardSizeNoMargins)
    { // No objects will be added if board is full.
        board[0][j].toDraw = APPLE;
        board[0][j].health = 0;
    }
}

unsigned int getNthEmpty(unsigned int n)
{
    unsigned int j;
    for (j = 0; j <= n && n < BOARD_SIZE_NO_MARGINS; j++)
    {
        if ((board[0][j].toDraw == APPLE || !(board[0][j].health == 0 || (board[0][j].health == 1 && board[0][j].toDraw == BLANK)))) // inefficient to look through whole array. Also inefficient to ask for more random numbers if spot selected is occupied. This was chosen because our rng is not particularly random, so repeated calls not particularly good.
            n++;                                                                                                                     // only advances position if there is not an object on the board. This serves to select the n-th empty tile
    }
    return j - 1;
}
char isHead(tile t)
{
    return t.toDraw == HEAD;
}
char isTail(tile t)
{
    return t.toDraw == TAIL;
}
signed int parseDirX(DIRECTION dir)
{
    if (dir == LEFT)
        return -1;
    if (dir == RIGHT)
        return 1;
    return 0;
}
signed int parseDirY(DIRECTION dir)
{
    if (dir == DOWN)
        return 1;
    if (dir == UP)
        return -1;
    return 0;
}
tile *getBoard()
{
    return (tile *)board;
}
snake *getSnakes()
{
    return snakes;
}