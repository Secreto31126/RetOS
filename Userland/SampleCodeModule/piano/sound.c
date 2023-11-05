#include "sound.h"

#define SHH 0
#define C3 262
#define C3sus 278
#define D3 294
#define D3sus 312
#define E3 330
#define F3 350
#define F3sus 370
#define G3 392
#define G3sus 416
#define A3 440
#define A3sus 466
#define B3 494
#define C4 524
#define C4sus 554
#define D4 588
#define D4sus 622
#define E4 660
#define F4 698
#define F4sus 740
#define G4 784
#define G4sus 830
#define A4 880
#define A4sus 932
#define B4 988
#define C5 1046

// #define MEEP(f, d) \
//     beep(f);          \
//     sleep_ticks(d);

/**
 * @see https://github.com/gabszzz/The-Imperial-March
 */
void imperial_march()
{
    playFor(A3, 14);
    playFor(SHH, 3);
    playFor(A3, 14);
    playFor(SHH, 3);
    playFor(A3, 14);
    playFor(SHH, 10);
    playFor(F3, 5);
    playFor(C4, 4);
    playFor(A3, 14);
    playFor(F3, 5);
    playFor(C4, 4);
    playFor(A3, 22);
    playFor(SHH, 10);
    playFor(E4, 12);
    playFor(E4, 12);
    playFor(E4, 12);
    playFor(SHH, 10);
    playFor(F4, 6);
    playFor(D4, 5);
    playFor(A3, 12);
    playFor(F3, 5);
    playFor(C4, 5);
    playFor(A3, 22);

    beep(SHH);
}

void hes_a_pirate()
{
    playFor(E3, 3);
    playFor(G3, 3);
    playFor(A3, 3);
    playFor(SHH, 3);
    playFor(A3, 7);

    playFor(A3, 3);
    playFor(B3, 3);
    playFor(C4, 3);
    playFor(SHH, 3);
    playFor(C4, 7);

    playFor(C4, 3);
    playFor(D4, 3);
    playFor(B3, 3);
    playFor(SHH, 3);
    playFor(B3, 7);

    playFor(A3, 3);
    playFor(G3, 3);
    playFor(A3, 3);

    playFor(SHH, 10);

    playFor(E3, 3);
    playFor(G3, 3);
    playFor(A3, 3);
    playFor(SHH, 3);
    playFor(A3, 7);

    playFor(A3, 3);
    playFor(B3, 3);
    playFor(C4, 3);
    playFor(SHH, 3);
    playFor(C4, 7);

    playFor(C4, 3);
    playFor(D4, 3);
    playFor(B3, 3);
    playFor(SHH, 3);
    playFor(B3, 7);

    playFor(G3, 3);
    playFor(A3, 3);

    playFor(SHH, 10);

    playFor(E3, 3);
    playFor(G3, 3);
    playFor(A3, 3);
    playFor(SHH, 3);
    playFor(A3, 7);

    playFor(A3, 3);
    playFor(B3, 3);
    playFor(C4, 3);
    playFor(SHH, 3);
    playFor(C4, 7);

    playFor(C4, 3);
    playFor(D4, 3);
    playFor(E4, 3);
    playFor(SHH, 3);
    playFor(E4, 3);

    playFor(D4, 3);
    playFor(C4, 3);
    playFor(D4, 3);
    playFor(A3, 7);

    playFor(SHH, 10);

    playFor(A3, 3);
    playFor(B3, 3);
    playFor(C4, 3);
    playFor(SHH, 3);
    playFor(C4, 7);

    playFor(D4, 7);
    playFor(E4, 4);
    playFor(A3, 7);

    playFor(A3, 3);
    playFor(C4, 3);
    playFor(B3, 18);
    playFor(SHH, 3);
    playFor(B3, 7);
    playFor(C4, 3);
    playFor(A3, 3);
    playFor(B3, 7);

    beep(SHH);
}

void outer_wilds()
{
    playFor(E3, 10);
    playFor(A3, 10);
    playFor(C4sus, 12);

    playFor(A3, 7);
    playFor(D4, 7);
    playFor(C4sus, 7);
    playFor(B3, 7);
    playFor(A3, 7);
    playFor(B3, 7);
    playFor(C4sus, 7);
    playFor(A3, 10);

    playFor(E3, 10);
    playFor(A3, 10);
    playFor(C4sus, 12);

    playFor(A3, 7);
    playFor(D4, 7);
    playFor(C4sus, 7);
    playFor(B3, 7);
    playFor(A3, 7);
    playFor(B3, 7);
    playFor(C4sus, 7);
    playFor(E4, 7);
    playFor(C4sus, 7);

    playFor(E3, 10);
    playFor(A3, 10);
    playFor(C4sus, 12);

    playFor(A3, 7);
    playFor(D4, 7);
    playFor(C4sus, 7);
    playFor(B3, 7);
    playFor(A3, 7);
    playFor(B3, 7);
    playFor(C4sus, 7);
    playFor(A3, 10);

    playFor(E3, 10);
    playFor(A3, 10);
    playFor(C4sus, 12);

    playFor(A3, 7);
    playFor(D4, 7);
    playFor(C4sus, 7);
    playFor(B3, 7);
    playFor(A3, 7);
    playFor(B3, 7);
    playFor(C4sus, 7);
    playFor(E4, 7);
    playFor(C4sus, 7);

    playFor(E4, 7);
    playFor(C4sus, 7);
    playFor(E4, 7);
    playFor(C4sus, 7);

    playFor(A3, 7);
    playFor(C4sus, 7);
    playFor(A3, 7);
    playFor(C4sus, 7);

    playFor(E4, 7);
    playFor(C4sus, 7);
    playFor(E4, 7);
    playFor(C4sus, 7);

    playFor(A3, 7);
    playFor(C4sus, 7);
    playFor(A3, 7);
    playFor(C4sus, 7);

    playFor(E4, 7);
    playFor(C4sus, 7);
    playFor(E4, 7);
    playFor(C4sus, 7);

    playFor(A3, 7);
    playFor(C4sus, 7);
    playFor(A3, 7);
    playFor(C4sus, 7);

    playFor(E4, 7);
    playFor(C4sus, 7);
    playFor(E4, 7);
    playFor(C4sus, 7);

    beep(SHH);
}

void do_i_wanna_know()
{
    playFor(F3, 3);
    playFor(G3, 3);
    playFor(A3sus, 8);
    playFor(G3, 18);

    playFor(SHH, 18);

    playFor(F3, 3);
    playFor(G3, 3);
    playFor(A3sus, 5);
    playFor(F4, 6);
    playFor(D4sus, 18);
    playFor(D4, 6);
    playFor(C4, 18);

    playFor(SHH, 3);

    playFor(A3sus, 3);
    playFor(C4, 3);
    playFor(D4, 3);
    playFor(C4, 9);

    playFor(SHH, 6);

    playFor(A3sus, 4);
    playFor(C4, 4);
    playFor(D4, 4);
    playFor(SHH, 2);
    playFor(D4, 4);
    playFor(G3, 18);

    playFor(SHH, 9);

    playFor(F3, 3);
    playFor(G3, 3);
    playFor(A3sus, 8);
    playFor(G3, 18);

    beep(SHH);
}

void sports_center()
{
    playFor(G4, 3);
    playFor(F4, 3);
    playFor(G4, 3);

    playFor(SHH, 7);

    playFor(G4, 3);
    playFor(F4, 3);
    playFor(G4, 3);

    shut();
}

void here_comes_the_sun()
{
    playFor(B4, 9);
    playFor(A4, 9);
    playFor(B4, 9);
    playFor(G4, 9);

    playFor(SHH, 7);

    playFor(B4, 7);
    playFor(G4, 9);
    playFor(A4, 9);
    playFor(B4, 9);

    playFor(SHH, 2);

    playFor(B4, 9);
    playFor(A4, 9);
    playFor(B4, 9);
    playFor(G4, 12);

    playFor(SHH, 2);

    playFor(G4, 9);
    playFor(A4, 9);
    playFor(G4, 9);

    playFor(SHH, 9);

    playFor(B4, 9);
    playFor(A4, 9);
    playFor(G4, 9);

    beep(SHH);
}

/**
 * @brief Let's a sound start playing (should be shut down with shut())
 *
 * @param freq frecuency of the sound
 */
void play(uint32_t freq)
{
    beep(freq);
}

/**
 * @brief  Stops the sound
 *
 */
void shut()
{
    beep(0);
}

/**
 * @brief Plays a sound for a certain amount of time
 *
 * @param freq frecuency of the sound
 * @param ticks amount of ticks the sound will be played
 */
void playFor(uint32_t freq, uint32_t ticks)
{
    uint32_t start = get_tick();
    beep(freq);
    sleep(ticks);
    shut();
}

/**
 * @brief Plays sounds of different frecuencies for 2 ticks each, in a loop for a certain amount of time
 *
 * @param freqs frecuencies of the sounds
 * @param cant amount of frecuencies
 * @param length amount of ticks the sound will be played
 */
void playSwitch(uint32_t *freqs, uint32_t cant, uint32_t length)
{
    uint32_t start = get_tick();
    uint32_t i = 0;
    while (get_tick() - start < length)
    {
        playFor(freqs[i++ % cant], 2);
    }
    shut();
}