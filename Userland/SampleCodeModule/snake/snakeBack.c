#include "snake.h"
#include "snakePrivate.h"
#include "random.h"

#define APPLE (PLAYER_COUNT + 1)
#define EMPTY 0
#define BOARD_SIZE (BOARD_HEIGHT * BOARD_WIDTH)
#define BOARD_SIZE_NO_MARGINS ((BOARD_HEIGHT - 2) * (BOARD_WIDTH - 2))

static tile board[BOARD_HEIGHT][BOARD_WIDTH] = {{0}};
static snake snakes[PLAYER_COUNT];
static unsigned int objects;

unsigned int getNthEmpty(unsigned int n);
char isTail(unsigned int identifier, unsigned int health);
char isHead(unsigned int identifier, unsigned int health);
signed int parseDirX(DIRECTION dir);
signed int parseDirY(DIRECTION dir);
void killSnake(unsigned int identifier);
void growSnake(unsigned int identifier);
void makeApple();

void setBoard(int snakeCount)
{
    objects = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        board[0][i].health = 0;
        board[0][i].identifier = EMPTY;
    }

    uint64_t boardSizeNoMargins = BOARD_SIZE_NO_MARGINS; // just to avoid calculating it in every loop
    for (int i = 0; i < APPLE_COUNT; i++)
    {
        unsigned int pos = randBetween(0, boardSizeNoMargins - i); // no objects will be placed on edges on setting board.
        unsigned int j = getNthEmpty(pos);
        if (j < boardSizeNoMargins)
        {
            board[0][j].identifier = APPLE; // No objects will be added if board is full.
            board[0][j].toDraw = SHEEP;
        }
    }
    for (int i = 1; i <= snakeCount; i++)
    {
        paintChar("p", -1, 0);
        unsigned int pos = randBetween(0, boardSizeNoMargins - i - APPLE_COUNT); // no objects will be placed on edges on setting board.
        unsigned int j = getNthEmpty(pos);
        if (j < boardSizeNoMargins)
        {
            board[0][j].identifier = i; // No objects will be added if board is full. Snake n represented by numbers k such that k = (n+1) modulo (PLAYER_COUNT+1). 1 represents an apple.
            board[0][j].health = START_PLAYER_LENGTH;
            board[0][j].toDraw = HEAD;
        }
        snakes[i - 1].length = START_PLAYER_LENGTH;
        snakes[i - 1].color = getHexColor();
        snakes[i - 1].direction = ((j % BOARD_WIDTH) > (BOARD_WIDTH / 2)) ? LEFT : RIGHT; // This way you don't start moving towards the edge. Still won't spawn at the edge out of respect.
        snakes[i - 1].alive = 1;
        snakes[i - 1].moved = 0;
    }
    objects = APPLE_COUNT + PLAYER_COUNT * START_PLAYER_LENGTH; // counts non-empty tiles, useful for resetting apples
}

void setDirection(unsigned int playerNumber, DIRECTION direction)
{
    DIRECTION prev = snakes[playerNumber % PLAYER_COUNT].direction;
    if (!(prev == UP && direction == DOWN) && !(prev == DOWN && direction == UP) && !(prev == LEFT && direction == RIGHT) && !(prev == RIGHT && direction == LEFT))
        snakes[playerNumber % PLAYER_COUNT].direction = direction;
}

// returns 0 if no players died. Returns the player identifier if a player died.
unsigned int update()
{
    char toReturn = 0;
    tile lookingAt;
    for (int i = 0; i < BOARD_HEIGHT; i++)
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            lookingAt = board[i][j];
            if (lookingAt.identifier != EMPTY && lookingAt.identifier != APPLE)
            {                                                                                                  // not empty and not an apple -> is a snake
                if (isHead(lookingAt.identifier, lookingAt.health) && !snakes[lookingAt.identifier - 1].moved) // prevents moving a newly formed head
                {
                    int nextX = j + parseDirX(snakes[lookingAt.identifier - 1].direction);
                    int nextY = i + parseDirY(snakes[lookingAt.identifier - 1].direction);

                    if (nextX < 0 || nextX >= BOARD_WIDTH || nextY < 0 || nextY >= BOARD_HEIGHT || (board[nextY][nextX].identifier != EMPTY && board[nextY][nextX].identifier != APPLE))
                    {
                        char aux[10];
                        paintString(itoa(nextX, aux, 10), 0xFFFFFFFF, 0xFF00FFFF);
                        paintString("|", 0xFFFFFFFF, 0xFF00FFFF);
                        paintString(itoa(nextY, aux, 10), 0xFFFFFFFF, 0xFF00FFFF);
                        paintString("|", 0xFFFFFFFF, 0xFF00FFFF);
                        paintString(itoa(i, aux, 10), 0xFFFFFFFF, 0xFF00FFFF);
                        paintString("dir:", 0xFFFFFFFF, 0xFF00FFFF);
                        paintString(itoa(snakes[lookingAt.identifier - 1].direction, aux, 10), 0xFFFFFFFF, 0xFF00FFFF);
                        killSnake(lookingAt.identifier);
                        toReturn = lookingAt.identifier;
                    }
                    else
                    {
                        if (board[nextY][nextX].identifier == APPLE)
                        {
                            growSnake(lookingAt.identifier);
                            makeApple();
                        }
                        board[nextY][nextX].identifier = lookingAt.identifier;
                        DIRECTION aux = snakes[lookingAt.identifier - 1].direction;
                        board[nextY][nextX].health = snakes[lookingAt.identifier - 1].length + (aux == RIGHT || aux == DOWN) ? 1 : 0; // board is analyzed left-right and up-down. A newly created head to the right or down will be shrunk incorrectly otherwise
                        snakes[lookingAt.identifier - 1].moved = 1;                                                                   // ensures no snake moves twice in a turn
                        snakes[lookingAt.identifier - 1].lastMove = aux;
                    }
                }
                else if (isTail(lookingAt.identifier, lookingAt.health))
                {
                    board[i][j].identifier = EMPTY;
                    board[i][j].toDraw = BLANK;
                }
                board[i][j].health--; // All snake parts lose one 'health' per movement. This way, parts remain for as many movements as the snake is long, giving the appearance of a continuous snake. Using identifiers to uniformly color snakes reinforces this
                if (isTail(board[i][j].identifier, board[i][j].health))
                    board[i][j].toDraw = TAIL;
                else
                    board[i][j].toDraw = BODY;
            }
            else if (lookingAt.identifier == EMPTY)
            {
                board[i][j].toDraw = NO_DRAW;
            }
        }
    for (int i = 0; i < PLAYER_COUNT; i++)
        snakes[i].moved = 0; // resets all snake movements
    return toReturn;         // identifier of dead snake, or 0
}

char isHead(unsigned int identifier, unsigned int health)
{
    return identifier != EMPTY && identifier != APPLE && (snakes[(identifier - 1) % PLAYER_COUNT].length == health);
}
char isTail(unsigned int identifier, unsigned int health)
{
    return identifier != EMPTY && identifier != APPLE && health == 1;
}
tile *getBoard()
{
    return (tile *)board;
}
snake *getSnakes()
{
    return snakes;
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
void killSnake(unsigned int identifier)
{
    for (int i = 0; i < BOARD_SIZE; i++)
        if (board[0][i].identifier == identifier)
        {
            board[0][i].identifier = EMPTY;
            board[0][i].toDraw = BLANK;
        }
    snakes[identifier - 1].alive = 0;
}
void growSnake(unsigned int identifier)
{
    for (int i = 0; i < BOARD_SIZE; i++)
        if (board[0][i].identifier == identifier)
            board[0][i].health++;
    snakes[identifier - 1].length++;
    objects++;
}
void makeApple()
{
    uint64_t boardSizeNoMargins = BOARD_SIZE_NO_MARGINS;
    unsigned int pos = randBetween(0, boardSizeNoMargins - objects); // no objects will be placed on edges on setting board.
    unsigned int j = getNthEmpty(pos);
    if (j < boardSizeNoMargins)
    {
        board[0][j].identifier = APPLE; // No objects will be added if board is full.
        board[0][j].toDraw = SHEEP;
    }
}

unsigned int getNthEmpty(unsigned int n)
{
    unsigned int j;
    for (j = 0; j < n && n < BOARD_SIZE_NO_MARGINS; j++)
    {
        if (board[j] != 0) // inefficient to look through whole array. Also inefficient to ask for more random numbers if spot selected is occupied. This was chosen because our rng is not particularly well implemented, so repeated calls not particularly good.
            n++;           // only advances position if there is not an object on the board. This serves to select the n-th empty tile
    }
    return j;
}