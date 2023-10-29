#include "snake.h"
#include "snakePrivate.h"
#include "./../window/backgroundArrays.h"
#define MOVE_INTERVAL 3

typedef struct frontSnake
{
    DIRECTION nextMove;
    HexColor bodyColor;
    HexColor otherColor;
} frontSnake;

void doMovement(char c, frontSnake *snakes);
void putDeath(int snakeNumber);
void drawBoard(frontSnake *snakes);
void drawBackground();
static ShapeFunction backgroundFunction = RetOSBackground;

char timeHasPassed(uint64_t start, uint64_t unit)
{
    return (get_tick() - start) > unit;
}

// As we do not currently have a single draw mode
void drawBackgroundWithParameters(Window w, uint64_t xOffset, uint64_t yOffset)
{
    // source = background;
    // toHexArray(source, w.pixels, DRAW_SIZE, DRAW_SIZE, w.width, w.height, 1, BACKGROUND_COLOR);

    overlayOnWindow(w, backgroundFunction, xOffset, yOffset, 1.0, 1.0, OPAQUE);

    // overlayFromCharArray(w, windowsArray, WINDOWS_WIDTH, WINDOWS_HEIGHT, windowsColorMap, xOffset, yOffset, OPAQUE); Never quite got this to work
}

int playSnake(uint16_t snakeCount)
{
    char gameOver = 0;
    int deadSnake = 0;
    uint16_t deathCount = 0;
    setBoard(snakeCount);

    frontSnake *snakes = malloc(2 * sizeof(frontSnake)); // Still saves space for second snake. Simpler than checking snakeCount for every non-complex operation
    for (int i = 0; i < snakeCount; i++)
    {
        snakes[i].nextMove = NONE;
        snakes[i].bodyColor = getHexColor();
        snakes[i].otherColor = getHexColor();
    }
    setSnakeDrawings();
    drawBackground();
    drawBoard(snakes);

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
    blank();
    paintString("Game Over. Press a key to return to shell.", 0xFFFF0000, 0);
    getChar();
    free(snakes);
    freeSnakeDrawings();
    blank();
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
    for (int i = 0; i < (BOARD_HEIGHT * BOARD_WIDTH); i++)
    {
        snakeDrawing currentDrawing = getCurrentDrawing();
        int drawSize = currentDrawing.drawSize;
        switch (board[i].toDraw)
        {
        case HEAD:
            source = currentDrawing.drawings[HEAD_D];
            toHexArray(source, stamp.pixels, drawSize, drawSize, stamp.width, stamp.height, 3, snakes[board[i].player].bodyColor, snakes[board[i].player].otherColor, 0xFFFF0000);
            break;
        case TAIL:
            drawBackgroundWithParameters(stamp, (i % BOARD_WIDTH) * tileWidth, (i / BOARD_WIDTH) * tileHeight);
            drawWindow(stamp, (i % BOARD_WIDTH) * tileWidth, (i / BOARD_WIDTH) * tileHeight); // Since the tail has transparency, the background must be redrawn before drawing tail.
            source = currentDrawing.drawings[TAIL_D];
            toHexArray(source, stamp.pixels, drawSize, drawSize, stamp.width, stamp.height, 3, BACKGROUND_COLOR, snakes[board[i].player].bodyColor, snakes[board[i].player].otherColor);
            break;
        case BODY:
            source = currentDrawing.drawings[BODY_D];
            toHexArray(source, stamp.pixels, drawSize, drawSize, stamp.width, stamp.height, 2, snakes[board[i].player].bodyColor, snakes[board[i].player].otherColor);
            break;
        case APPLE:
            source = currentDrawing.drawings[APPLE_D];
            toHexArray(source, stamp.pixels, drawSize, drawSize, stamp.width, stamp.height, 4, BACKGROUND_COLOR, APPLE_RED, APPLE_BROWN, APPLE_GREEN);
            break;
        case BLANK:
            drawBackgroundWithParameters(stamp, (i % BOARD_WIDTH) * tileWidth, (i / BOARD_WIDTH) * tileHeight);
            break;
        case NO_DRAW:
        default:
            continue;
            break;
        }
        if (board[i].toDraw != APPLE && board[i].toDraw != BLANK) // don't want spinning apples.
            switch (board[i].drawDirection)
            {
            case LEFT:
                rotateBy270(stamp); // Probably should make other rotations. But this is not a bottleneck and also works fine for now.
                break;
            case DOWN:
                rotateBy180(stamp);
                break;
            case RIGHT:
                rotateBy90(stamp);
                break;
            case UP:
            default:
                break;
            }
        drawWindow(stamp, (i % BOARD_WIDTH) * tileWidth, (i / BOARD_WIDTH) * tileHeight);
    }
    freeWindow(stamp);
    freeBack();
}

void drawBackground()
{
    uint64_t tileWidth = getScreenWidth() / BOARD_WIDTH, tileHeight = getScreenHeight() / BOARD_HEIGHT;
    Window stamp = getWindow(tileWidth, tileHeight, malloc(tileWidth * tileHeight * sizeof(HexColor)));
    tile *board = getBoard();
    for (int i = 0; i < (BOARD_HEIGHT * BOARD_WIDTH); i++)
    {
        drawBackgroundWithParameters(stamp, (i % BOARD_WIDTH) * tileWidth, (i / BOARD_WIDTH) * tileHeight);
        drawWindow(stamp, (i % BOARD_WIDTH) * tileWidth, (i / BOARD_WIDTH) * tileHeight); // Since the tail has transparency, the background must be redrawn before drawing tail.
    }
}

void setBackground(ShapeFunction newBackground)
{
    backgroundFunction = newBackground;
}