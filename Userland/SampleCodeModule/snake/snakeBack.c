#include "snake.h"
#include "snakePrivate.h"
#include "random.h"

#define EMPTY 0
#define BOARD_SIZE (BOARD_HEIGHT * BOARD_WIDTH)
#define BOARD_SIZE_NO_MARGINS ((BOARD_HEIGHT - 2) * (BOARD_WIDTH - 2))

static tile board[BOARD_HEIGHT][BOARD_WIDTH];
static snake snakes[PLAYER_COUNT];
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
    objects = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        board[0][i].health = 0;
        board[0][i].player = 0;
    }

    uint64_t boardSizeNoMargins = BOARD_SIZE_NO_MARGINS; // just to avoid calculating it in every loop
    for (int i = 0; i < APPLE_COUNT; i++)
    {
        unsigned int pos = randBetween(0, boardSizeNoMargins - i); // no objects will be placed on edges on setting board.
        unsigned int j = getNthEmpty(pos);
        if (j < boardSizeNoMargins)
        { // No objects will be added if board is full.
            board[0][j].toDraw = APPLE;
        }
    }
    for (int i = 0; i < snakeCount; i++)
    {
        paintChar("p", -1, 0);
        unsigned int pos = randBetween(0, boardSizeNoMargins - i - APPLE_COUNT); // no objects will be placed on edges on setting board.
        unsigned int j = getNthEmpty(pos);
        if (j < boardSizeNoMargins)
        {
            board[0][j].player = i; // No objects will be added if board is full. Snake n represented by numbers k such that k = (n+1) modulo (PLAYER_COUNT+1). 1 represents an apple.
            board[0][j].health = START_PLAYER_LENGTH;
            board[0][j].toDraw = HEAD;
        }
        snakes[i].length = START_PLAYER_LENGTH;
        snakes[i].color = getHexColor();
        snakes[i].direction = ((j % BOARD_HEIGHT) > (BOARD_WIDTH / 2)) ? LEFT : RIGHT; // This way you don't start moving towards the edge. Still won't spawn at the edge out of respect.
        snakes[i].alive = 1;
    }
    objects = APPLE_COUNT + snakeCount * START_PLAYER_LENGTH; // counts non-empty tiles, useful for resetting apples
}

void setDirection(unsigned int playerNumber, DIRECTION direction)
{
    DIRECTION prev = snakes[playerNumber % PLAYER_COUNT].direction;
    if (!(prev == UP && direction == DOWN) && !(prev == DOWN && direction == UP) && !(prev == LEFT && direction == RIGHT) && !(prev == RIGHT && direction == LEFT) && direction != NONE)
        snakes[playerNumber % PLAYER_COUNT].direction = direction;
}

// returns 0 if no players died. Returns the player player if a player died.
unsigned int update(int snakeCount)
{
    paintString("inBack", 0xFFFFFF00, 0xFF00FF00);
    char toReturn = 0;
    tile lookingAt;
    for (int i = 0; i < BOARD_HEIGHT; i++)
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            lookingAt = board[i][j];
            if (lookingAt.health > 0) // All else should have health 0
            {
                if (isHead(lookingAt))
                {
                    int nextX = j + parseDirX(snakes[lookingAt.player].direction);
                    int nextY = i + parseDirY(snakes[lookingAt.player].direction);
                    if (nextX < 0 || nextX >= BOARD_WIDTH || nextY < 0 || nextY >= BOARD_HEIGHT || board[nextY][nextX].health != 0)
                    {
                        killSnake(lookingAt.player);
                        toReturn = lookingAt.player + 1;
                    }
                    else
                    {
                        if (board[nextY][nextX].toDraw == APPLE)
                        {
                            growSnake(lookingAt.player);
                            makeApple();
                        }
                        snakes[lookingAt.player].nextHeadCoordinates[0] = nextX;
                        snakes[lookingAt.player].nextHeadCoordinates[1] = nextY;
                        snakes[lookingAt.player].lastMove = snakes[lookingAt.player].direction;
                        board[i][j].toDraw = BODY;
                    }
                }
                else if (isTail(lookingAt))
                {
                    board[i][j].toDraw = BLANK;
                }
                board[i][j].health--; // All snake parts lose one 'health' per movement. This way, parts remain for as many movements as the snake is long, giving the appearance of a continuous snake. Using players to uniformly color snakes reinforces this
                if (lookingAt.health == 1)
                    board[i][j].toDraw = TAIL;
            }
            else if (lookingAt.toDraw != APPLE) // apples redrawn at every turn currently. Prevents not drawing new apples
            {
                board[i][j].toDraw = NO_DRAW;
            }
        }
    setNewHeads(snakeCount);
    return toReturn; // player of dead snake, or 0
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
        }
    }
}

void killSnake(unsigned int player)
{
    paintString("death of:", -1, 0);
    paintChar(player + '0', 0xFFFF0000, 0xFF000000);
    for (int i = 0; i < BOARD_SIZE; i++)
        if (board[0][i].player == player && board[0][i].health != 0)
        {
            board[0][i].toDraw = BLANK;
            board[0][i].health = 0;
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
    unsigned int pos = randBetween(0, boardSizeNoMargins - objects); // no objects will be placed on edges on setting board.
    unsigned int j = getNthEmpty(pos);
    if (j < boardSizeNoMargins)
    { // No objects will be added if board is full.
        board[0][j].toDraw = APPLE;
        board[0][j].health = 0;
    }
}

unsigned int getNthEmpty(unsigned int n)
{
    unsigned int j;
    for (j = 0; j < n && n < BOARD_SIZE_NO_MARGINS; j++)
    {
        if (board[0][j].toDraw != APPLE && board[0][j].health == 0) // inefficient to look through whole array. Also inefficient to ask for more random numbers if spot selected is occupied. This was chosen because our rng is not particularly random, so repeated calls not particularly good.
            n++;                                                    // only advances position if there is not an object on the board. This serves to select the n-th empty tile
    }
    return j;
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
        return -1;
    if (dir == UP)
        return 1;
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