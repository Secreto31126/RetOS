#include "snake.h"
#include "snakePrivate.h"
#define MOVE_INTERVAL 2

typedef struct frontSnake
{
    DIRECTION nextMove;
    HexColor bodyColor;
    HexColor otherColor;
} frontSnake;

void doMovement(char c, frontSnake *snakes);
void putDeath(int snakeNumber);
void drawBoard(frontSnake *snakes);

char timeHasPassed(uint64_t start, uint64_t unit)
{
    return (get_tick() - start) > unit;
}
int playSnake(uint16_t snakeCount)
{
    char gameOver = 0;
    int deadSnake = 0;
    uint16_t deathCount = 0;

    if (snakeCount > 2)
        snakeCount = 2;
    setBoard(snakeCount);

    frontSnake *snakes = malloc(2 * sizeof(frontSnake)); // Still saves space for second snake. Simpler than checking snakeCount for every non-complex operation
    for (int i = 0; i < snakeCount; i++)
    {
        snakes[i].nextMove = NONE;
        snakes[i].bodyColor = getHexColor();
        snakes[i].otherColor = getHexColor();
    }

    uint64_t time = get_tick();
    while (!gameOver)
    {
        char c;
        while ((c = readChar()))
        {
            doMovement(c, snakes);
            paintString("reading", 0xFFFF0000, 0xFF00FF00);
        }
        if (timeHasPassed(time, MOVE_INTERVAL))
        {
            paintString("time", 0xFFFF0000, 0xFF00FF00); // remove
            time = get_tick();
            for (int i = 0; i < snakeCount; i++)
                if (snakes[i].nextMove != NONE)
                    setDirection(i, snakes[i].nextMove);
            if ((deadSnake = update(snakeCount)))
            {
                putDeath(deadSnake);
                deathCount++;
                if (deathCount >= snakeCount)
                    gameOver = 1;
            }
            drawBoard(snakes);
        }
    }
    free(snakes);
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

void drawBoard(frontSnake *snakes)
{
    uint64_t tileWidth = getScreenWidth() / BOARD_WIDTH, tileHeight = getScreenHeight() / BOARD_HEIGHT;
    Window stamp = getWindow(tileWidth, tileHeight, malloc(tileWidth * tileHeight * sizeof(HexColor)));
    tile *board = getBoard();
    snake *backSnakes = getSnakes();
    char *source;
    char aux[20];
    for (int i = 0; i < (BOARD_HEIGHT * BOARD_WIDTH); i++)
    {
        /* // remove
                paintString("Will draw on: ", -1, 0);
                paintString(itoa((i % BOARD_HEIGHT) * tileWidth, aux, 10), -1, 0);
                paintString(",", -1, 0);
                paintString(itoa((i / BOARD_WIDTH) * tileHeight, aux, 10), -1, 0);
                paintString("i is: ", -1, 0);
                paintString(itoa(i, aux, 10), -1 * 3, 0);
                paintString("Making stamp", -1, 0);
                toHexArray(source, stamp.pixels, DRAW_SIZE, DRAW_SIZE, stamp.width, stamp.height, 1, 0x88FFFFFF);
                paintString("Drawing stamp", -1, 0);
                drawWindow(stamp, (i % BOARD_WIDTH) * tileWidth, (i / BOARD_WIDTH) * tileHeight);
                paintString("success", -1, 0);
        */
        //
        paintChar(board[i].toDraw + '0', -1, 0);
        paintChar('|', -1, 0);
        switch (board[i].toDraw)
        {
        case HEAD:
            switch (backSnakes[board[i].player].direction)
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
            toHexArray(source, stamp.pixels, DRAW_SIZE, DRAW_SIZE, stamp.width, stamp.height, 2, snakes[board[i].player].bodyColor, snakes[board[i].player].otherColor);
            drawWindow(stamp, (i % BOARD_HEIGHT) * tileWidth, (i / BOARD_WIDTH) * tileHeight);
            paintChar('h', -1, 0); // remove
            break;
        case TAIL:
        case BODY:
            source = classicOther;
            toHexArray(source, stamp.pixels, DRAW_SIZE, DRAW_SIZE, stamp.width, stamp.height, 2, snakes[board[i].player].bodyColor, snakes[board[i].player].otherColor);
            drawWindow(stamp, (i % BOARD_HEIGHT) * tileWidth, (i / BOARD_WIDTH) * tileHeight);
            paintChar('b', -1, 0); // remove
            break;
        case NO_DRAW:
            /*
                source = malloc(tileHeight * tileWidth * sizeof(HexColor));
                toHexArray(source, stamp.pixels, DRAW_SIZE, DRAW_SIZE, stamp.width, stamp.height, 1, 0x88FFFFFF);
                drawWindow(stamp, (i % BOARD_HEIGHT) * tileWidth, (i / BOARD_WIDTH) * tileHeight);
                free(source);
                */
            break;
        default:
            break;
        }
        //
    }
    paintString("Leaving", -1, 0);
    // free(source); // remove
    free(stamp.pixels);
}