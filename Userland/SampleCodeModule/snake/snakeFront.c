#include "snake.h"
#include "snakePrivate.h"
#include "./../piano/sound.h"
#include "./drawings/backgroundArrays.h"
#include "drawings/snakeDrawings.h"
#include "./../window/window.h"
#define MAX_SCORE_LENGTH 4
#define MAX_LETTER_SIZE 5
typedef struct frontSnake
{
    DIRECTION nextMove;
    HexColor *colorMap;
} frontSnake;

void doMovement(char c, uint16_t snakeCount, frontSnake *snakes);
void putDeath(int snakeNumber);
void drawBoard(frontSnake *snakes);
void drawBackground();
void freeColorMaps(int snakeCount, frontSnake *snakes);
void drawScore(uint64_t score);
void drawTextBackground(uint64_t size, uint64_t textLength);
// default theme is windows
static char *backgroundArray = (char *)windowsArray;
static HexColor *backgroundColorMap = windowsColorMap;
static char redrawBeforeBody = 0;
static char redrawBeforeTail = 1;
static char redrawBeforeTurn = 1;
static char redrawBeforeHead = 0;
static snakeDrawing currentDrawing = {
    .drawSize = DRAW_SIZE,
    .headDrawing = (char *)classicHeadUp,
    .bodyDrawing = (char *)classicOther,
    .tailDrawing = (char *)classicTail,
    .turnDrawing = (char *)classicTurn,
    .growItemDrawing = (char *)classicApple,
    .growItemColorMap = (HexColor *)appleColorMap,
};

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
    uint64_t oldSize = getSize();
    uint64_t score = 0;
    setSeed(get_tick());
    char gameOver = 0;
    int deadSnake = 0;
    uint16_t deathCount = 0;
    setBoard(snakeCount);

    frontSnake *snakes = malloc(snakeCount * sizeof(frontSnake)); // Still saves space for second snake. Simpler than checking snakeCount for every non-complex operation
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
    play(80);
    drawBackground();
    play(300);
    drawBoard(snakes);
    shut();

    uint64_t time = get_tick();
    while (!gameOver)
    {
        shut(); // stops any noises that begun on previous update loop
        drawScore(score);
        char c;
        while ((c = readChar()))
        {
            uint64_t size;
            if (c == '+' && (size = (getSize() + 1)) < MAX_LETTER_SIZE)
            {
                drawTextBackground(size, MAX_SCORE_LENGTH);
                setSize(size);
            }
            else if (c == '-' && (size = (getSize() - 1)) >= 1)
            {
                drawTextBackground(size + 1, MAX_SCORE_LENGTH);
                setSize(size);
            }
            doMovement(c, snakeCount, snakes);
        }
        if (timeHasPassed(time, MOVE_INTERVAL))
        {
            time = get_tick();
            for (int i = 0; i < snakeCount; i++)
                if (snakes[i].nextMove != NONE)
                    setDirection(i, snakes[i].nextMove);
            int deaths = 0;
            int madeApple = 0;
            if ((deadSnake = update(snakeCount, &deaths, &madeApple)))
            {
                putDeath(deadSnake);
                deathCount += deaths;
                if (deathCount >= snakeCount)
                    gameOver = 1;
            }
            if (madeApple)
            {
                score += APPLE_POINTS;
                drawTextBackground(getSize(), MAX_SCORE_LENGTH);
                play(330);
            }
            drawBoard(snakes);
        }
    }

    play(440);
    blank();
    shut();

    // drawBackground();  // undecided between a blank background or the snake background.
    while (readChar()) // empties out buffer, in case player pressed a key while background was being cleared (not necessary, just prevents skipping the game-over screen)
        ;
    setSize(oldSize);
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

void doMovement(char c, uint16_t snakeCount, frontSnake *snakes)
{
    switch (c)
    {
    case 'w':
        snakes[0].nextMove = UP;
        return;
    case 'a':
        snakes[0].nextMove = LEFT;
        return;
    case 's':
        snakes[0].nextMove = DOWN;
        return;
    case 'd':
        snakes[0].nextMove = RIGHT;
        return;
    default:
        break;
    }
    if (snakeCount >= 2)
    {
        switch (c)
        {
        case 'i':
            snakes[1].nextMove = UP;
            return;
        case 'j':
            snakes[1].nextMove = LEFT;
            return;
        case 'k':
            snakes[1].nextMove = DOWN;
            return;
        case 'l':
            snakes[1].nextMove = RIGHT;
            return;
        default:
            break;
        }
    }
    if (snakeCount >= 3)
    {
        if (c == 'v' || c == ' ')
            snakes[2].nextMove = (snakes[2].nextMove + 1) % NUMBER_OF_DIRECTIONS;
        else if (c == 'b')
            snakes[2].nextMove = (snakes[2].nextMove + NUMBER_OF_DIRECTIONS - 1) % NUMBER_OF_DIRECTIONS; // because c modulus doesn't handle negatives properly
    }
}

void drawBoard(frontSnake *snakes)
{
    uint64_t h = getScreenHeight(), w = getScreenWidth(), address = 0;
    uint64_t tileWidth = w / BOARD_WIDTH, tileHeight = h / BOARD_HEIGHT;
    Window stamp = getWindow(tileWidth, tileHeight, malloc(tileWidth * tileHeight * sizeof(HexColor)));
    tile *board = getBoard();
    // snake *backSnakes = getSnakes();
    char *source;
    uint64_t drawSize = currentDrawing.drawSize;
    for (int i = 0; i < h; i += tileHeight)
    {
        for (int j = 0; j < w; j += tileWidth, address++)
        {
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
            case TURN:
                if (redrawBeforeTurn)
                    drawBackgroundWithParameters(stamp, j, i); // Since the tail has transparency, the background must be redrawn before drawing tail if the head has color where the tail is transparent.
                source = currentDrawing.turnDrawing;
                fromCharArray(stamp, source, drawSize, drawSize, snakes[board[address].player].colorMap, 0, 0, OPAQUE);
                break;
            case BODY:
                if (redrawBeforeBody)
                    drawBackgroundWithParameters(stamp, j, i); // Since the body might have transparency, the background must be redrawn before drawing tail.
                source = currentDrawing.bodyDrawing;
                fromCharArray(stamp, source, drawSize, drawSize, snakes[board[address].player].colorMap, 0, 0, OPAQUE);
                break;
            case APPLE:
                if (!board[address].health)
                    continue;
                source = currentDrawing.growItemDrawing;
                fromCharArray(stamp, source, drawSize, drawSize, currentDrawing.growItemColorMap, 0, 0, OPAQUE);
                break;
            case BLANK:
                drawBackgroundWithParameters(stamp, j, i);
                continue;
                break;
            case NO_DRAW:
                /* uncomment to visualize what is not being drawn
                    source = currentDrawing.growItemDrawing;
                    HexColor allRed[] = {0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000, 0xFFFF0000};
                    fromCharArray(stamp, source, drawSize, drawSize, allRed, 0, 0, OPAQUE);
                    break;
                */
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
    uint64_t h = getScreenHeight(), w = getScreenWidth();
    /*uint64_t tileWidth = getScreenWidth() / BOARD_WIDTH, tileHeight = getScreenHeight() / BOARD_HEIGHT;
    Window stamp = getWindow(tileWidth, tileHeight, malloc(tileWidth * tileHeight * sizeof(HexColor)));
    for (int i = 0; i < h; i += tileHeight)
    {
        for (int j = 0; j < w; j += tileWidth)
        {
            drawBackgroundWithParameters(stamp, j, i);
            drawWindow(stamp, j, i); // Since the tail has transparency, the background must be redrawn before drawing tail.
        }
    }
    */
    Window wholeScreen = getWindow(w, h, malloc(h * w * sizeof(HexColor)));
    overlayFromCharArray(wholeScreen, backgroundArray, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, backgroundColorMap, 0, 0, OPAQUE);
    quickDraw(wholeScreen);
    freeWindow(wholeScreen);
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

void drawScore(uint64_t score)
{
    char aux[MAX_SCORE_LENGTH];
    uint64_t max = (pow(10, MAX_SCORE_LENGTH) - 1);
    score = score > max ? max : score;
    drawStringAt(itoa(score, aux, 10), SCORE_COLOR, 0, 0, 0);
}

void drawTextBackground(uint64_t size, uint64_t textLength)
{
    uint64_t widthSpan = TRUE_LETTER_WIDTH * size * textLength, heightSpan = TRUE_LETTER_HEIGHT * size, tileWidth = getScreenWidth() / BOARD_WIDTH, tileHeight = getScreenHeight() / BOARD_HEIGHT;
    Window stamp = getWindow(tileWidth, tileHeight, malloc(sizeof(HexColor) * tileWidth * tileHeight));
    for (int i = 0; i <= heightSpan; i += tileHeight)
    {
        for (int j = 0; j <= widthSpan; j += tileWidth)
            drawBackgroundWithParameters(stamp, j, i);
    }
    freeWindow(stamp);
}