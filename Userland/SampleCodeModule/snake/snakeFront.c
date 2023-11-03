#include "snake.h"
#include "snakePrivate.h"
#include "./../piano/sound.h"
#include "./drawings/backgroundArrays.h"
#include "drawings/snakeDrawings.h"

typedef struct frontSnake
{
    DIRECTION nextMove;
    HexColor *colorMap;
} frontSnake;

void doMovement(char c, frontSnake *snakes);
void putDeath(int snakeNumber);
void drawBoard(frontSnake *snakes);
void drawBackground();
void freeColorMaps(int snakeCount, frontSnake *snakes);
// default theme is windows
static char *backgroundArray = windowsArray;
static HexColor *backgroundColorMap = windowsColorMap;
static char redrawBeforeBody = 0;
static char redrawBeforeTail = 1;
static char redrawBeforeTurn = 1; // currently unused
static char redrawBeforeHead = 0;
static snakeDrawing currentDrawing = {DRAW_SIZE, classicHeadUp, classicOther, classicTail, 0, classicApple, appleColorMap}; // turn currently unused

char timeHasPassed(uint64_t start, uint64_t unit)
{
    return (get_tick() - start) > unit;
}

// Old draw modes commented porque me dio pena borrarlos :D
void drawBackgroundWithParameters(Window w, uint64_t xOffset, uint64_t yOffset)
{
    // source = background;
    // toHexArray(source, w.pixels, DRAW_SIZE, DRAW_SIZE, w.width, w.height, 1, BACKGROUND_COLOR);

    // overlayOnWindow(w, backgroundFunction, xOffset, yOffset, 1.0, 1.0, OPAQUE);

    // overlayFromCharArray(w, windowsArray, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, windowsColorMap, xOffset, yOffset, OPAQUE);

    overlayFromCharArray(w, backgroundArray, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, backgroundColorMap, xOffset, yOffset, OPAQUE);
    drawWindow(w, xOffset, yOffset);
}
int playSnake(uint16_t snakeCount)
{
    setSeed(get_tick());
    char gameOver = 0;
    int deadSnake = 0;
    uint16_t deathCount = 0;
    setBoard(snakeCount);

    frontSnake *snakes = malloc(2 * sizeof(frontSnake)); // Still saves space for second snake. Simpler than checking snakeCount for every non-complex operation
    for (int i = 0; i < snakeCount; i++)
    {
        snakes[i].nextMove = NONE;
        snakes[i].colorMap = malloc(MAX_SNAKE_COLORS * sizeof(HexColor));
        for (int j = 1; j < MAX_SNAKE_COLORS - 1; j++)
        {
            snakes[i].colorMap[j] = getHexColor();
        }
        snakes[i].colorMap[0] = 0x00000000;                              // First color is reserved for transparency
        snakes[i].colorMap[MAX_SNAKE_COLORS - 1] = 0xFF000000 | HEX_RED; // Last color is reserved for eyes, always red
    }
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

    play(440);
    blank();
    shut();

    // drawBackground();  // undecided between a blank background or the snake background.
    while (readChar()) // empties out buffer, in case player pressed a key while background was being cleared (not necessary, just prevents skipping the game over screen)
        ;
    paintString("Game Over. Press a key to return to shell.", 0xFF000000 | HEX_RED, 0xFF | HEX_BLACK);
    getChar();
    freeColorMaps(snakeCount, snakes);
    free(snakes);
    freeBack();
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
    uint64_t h = getScreenHeight(), w = getScreenWidth(), address = 0;
    uint64_t tileWidth = w / BOARD_WIDTH, tileHeight = h / BOARD_HEIGHT;
    Window stamp = getWindow(tileWidth, tileHeight, malloc(tileWidth * tileHeight * sizeof(HexColor)));
    tile *board = getBoard();
    snake *backSnakes = getSnakes();
    char *source;
    uint64_t drawSize = currentDrawing.drawSize;
    for (int i = 0; i < h; i += tileHeight)
    {
        for (int j = 0; j < w; j += tileWidth, address++)
        {
            char aux[10];
            switch (board[address].toDraw)
            {
            case HEAD:
                if (redrawBeforeHead)
                    drawBackgroundWithParameters(stamp, j, i); // Since the head can have transparency, the background must be redrawn before drawing head if a grow item has color where the head has transparency.
                source = currentDrawing.headDrawing;
                fromCharArray(stamp, source, drawSize, drawSize, snakes[board[address].player].colorMap, 0, 0, OPAQUE);
                break;
            case TAIL:
                if (redrawBeforeTail)
                    drawBackgroundWithParameters(stamp, j, i); // Since the tail has transparency, the background must be redrawn before drawing tail if the head has color where the tail is transparent.
                source = currentDrawing.tailDrawing;
                fromCharArray(stamp, source, drawSize, drawSize, snakes[board[address].player].colorMap, 0, 0, OPAQUE);
                break;
            case BODY:
                if (redrawBeforeBody)
                    drawBackgroundWithParameters(stamp, j, i); // Since the body might have transparency, the background must be redrawn before drawing tail.
                source = currentDrawing.bodyDrawing;
                fromCharArray(stamp, source, drawSize, drawSize, snakes[board[address].player].colorMap, 0, 0, OPAQUE);
                break;
            case APPLE:
                source = currentDrawing.growItemDrawing;
                fromCharArray(stamp, source, drawSize, drawSize, currentDrawing.growItemColorMap, 0, 0, OPAQUE);
                break;
            case BLANK:
                drawBackgroundWithParameters(stamp, j, i);
                continue;
                break;
            case NO_DRAW:
            default:
                continue;
                break;
            }
            if (board[address].toDraw != APPLE && board[i].toDraw != BLANK) // don't want spinning apples or backgrounds.
                switch (board[address].drawDirection)
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
            drawWindow(stamp, j, i);
        }
        address--;
    }
    freeWindow(stamp);
}

void drawBackground()
{
    uint64_t tileWidth = getScreenWidth() / BOARD_WIDTH, tileHeight = getScreenHeight() / BOARD_HEIGHT;
    uint64_t h = getScreenHeight(), w = getScreenWidth();
    Window stamp = getWindow(tileWidth, tileHeight, malloc(tileWidth * tileHeight * sizeof(HexColor)));
    for (int i = 0; i < h; i += tileHeight)
    {
        for (int j = 0; j < w; j += tileWidth)
        {
            drawBackgroundWithParameters(stamp, j, i);
            drawWindow(stamp, j, i); // Since the tail has transparency, the background must be redrawn before drawing tail.
        }
    }
}

void setBackgroundArray(char *newBackground)
{
    backgroundArray = newBackground;
}
void setBackgroundColorMap(HexColor *newBackground)
{
    backgroundColorMap = newBackground;
}
void setDrawOptions(char headRedraw, char bodyRedraw, char tailRedraw, char turnRedraw)
{
    redrawBeforeHead = headRedraw;
    redrawBeforeBody = bodyRedraw;
    redrawBeforeTail = tailRedraw;
    redrawBeforeTurn = turnRedraw;
}
void freeColorMaps(int snakeCount, frontSnake *snakes)
{
    for (int i = 0; i < snakeCount; i++)
        free(snakes[i].colorMap);
}
void setSnakeDrawing(char drawSize, char *headDrawing, char *bodyDrawing, char *tailDrawing, char *turnDrawing, char *growItemDrawing, HexColor *growItemColorMap)
{
    currentDrawing.drawSize = drawSize;
    currentDrawing.headDrawing = headDrawing;
    currentDrawing.bodyDrawing = bodyDrawing;
    currentDrawing.tailDrawing = tailDrawing;
    currentDrawing.turnDrawing = turnDrawing;
    currentDrawing.growItemDrawing = growItemDrawing;
    currentDrawing.growItemColorMap = growItemColorMap;
}