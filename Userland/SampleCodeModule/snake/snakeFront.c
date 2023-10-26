#include "snake.h"
#include "snakePrivate.h"
#define MOVE_INTERVAL 2

typedef struct frontSnake
{
    DIRECTION nextMove;
} frontSnake;
void doMovement(char c, frontSnake *snakes);
void putDeath(int snakeNumber);
char timeHasPassed(uint64_t start, uint64_t unit)
{
    return (get_tick() - start) > unit;
}
int playSnake(uint16_t snakeCount)
{
    uint64_t tileWidth = getScreenWidth() / BOARD_WIDTH, tileHeight = getScreenHeight() / BOARD_HEIGHT;
    Window stamp = getWindow(tileWidth, tileHeight, malloc(tileWidth * tileHeight * sizeof(HexColor)));

    char gameOver = 0;
    int deadSnake = 0;
    uint16_t deathCount = 0;

    if (snakeCount > 2)
        snakeCount = 2;
    setBoard(snakeCount);

    frontSnake *snakes = malloc(2 * sizeof(frontSnake)); // Still saves space for second snake. Simpler than checking snakeCount for every non-complex operation
    for (int i = 0; i < snakeCount; i++)
        snakes[i].nextMove = NONE;

    uint64_t time = get_tick();
    while (!gameOver)
    {
        char c;
        while ((c = readChar()))
        {
            doMovement(c, snakes);
        }
        if (timeHasPassed(time, MOVE_INTERVAL))
        {
            paintChar('i', 0xFFFF0000, 0xFF00FF00); // remove
            time = get_tick();
            for (int i = 0; i < snakeCount; i++)
                if (snakes[i].nextMove != NONE)
                    setDirection(i, snakes[i].nextMove);
            deadSnake = update();
            if (deadSnake)
            {
                putDeath(deadSnake);
                deathCount++;
                if (deathCount >= snakeCount)
                    gameOver = 1;
            }
            tile *board = getBoard();
            snake *backSnakes = getSnakes();
            for (int i = 0; i < BOARD_HEIGHT * BOARD_WIDTH; i++)
            {
                char *source;
                switch (board[i].toDraw)
                {
                case HEAD:
                    switch (backSnakes[board[i].identifier - 1].direction)
                    {
                    case LEFT:
                        source = classicHeadLeft;
                        break;
                    case RIGHT:
                        source = classicHeadRight;
                        break;
                    case DOWN:
                        source = classicHeadDown;
                        break;
                    case UP:
                    default:
                        source = classicHeadUp;
                        break;
                    }
                    toHexArray(source, stamp.pixels, DRAW_SIZE, DRAW_SIZE, stamp.width, stamp.height, 2, 0xFF00FF00, 0xFFFF0000);
                    drawWindow(stamp, (i % BOARD_HEIGHT) * tileWidth, (i / BOARD_WIDTH) * tileHeight);
                    paintChar('h', -1, 0); // remove
                    break;
                case TAIL:
                case BODY:
                    source = classicOther;
                    toHexArray(source, stamp.pixels, DRAW_SIZE, DRAW_SIZE, stamp.width, stamp.height, 2, 0xFF00FF00, 0xFFFF0000);
                    drawWindow(stamp, (i % BOARD_HEIGHT) * tileWidth, (i / BOARD_WIDTH) * tileHeight);
                    paintChar('b', -1, 0); // remove
                    break;
                case NO_DRAW:
                default:
                    break;
                }
            }
        }
    }
    free(snakes);
    free(stamp.pixels);
    return deadSnake;
}
void putDeath(int snakeNumber)
{
}
void doMovement(char c, frontSnake *snakes)
{
    switch (c)
    {
    case 'w':
        snakes[0].nextMove = UP;
        break;
    case 'a':
        snakes[0].nextMove = LEFT;
        break;
    case 's':
        snakes[0].nextMove = DOWN;
        break;
    case 'd':
        snakes[0].nextMove = RIGHT;
        break;
    case 'i':
        snakes[1].nextMove = UP;
        break;
    case 'j':
        snakes[1].nextMove = LEFT;
        break;
    case 'k':
        snakes[1].nextMove = DOWN;
        break;
    case 'l':
        snakes[1].nextMove = RIGHT;
        break;
    default:
        break;
    }
}