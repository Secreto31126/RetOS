#include "audio.h"

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

#define MEEP(f, d) \
    beep(f);       \
    sleep_ticks(d);

/**
 * @see https://github.com/gabszzz/The-Imperial-March
 */
void imperial_march()
{
    MEEP(A3, 14);
    MEEP(SHH, 3);
    MEEP(A3, 14);
    MEEP(SHH, 3);
    MEEP(A3, 14);
    MEEP(SHH, 10);
    MEEP(F3, 5);
    MEEP(C4, 4);
    MEEP(A3, 14);
    MEEP(F3, 5);
    MEEP(C4, 4);
    MEEP(A3, 22);
    MEEP(SHH, 10);
    MEEP(E4, 12);
    MEEP(E4, 12);
    MEEP(E4, 12);
    MEEP(SHH, 10);
    MEEP(F4, 6);
    MEEP(D4, 5);
    MEEP(A3, 12);
    MEEP(F3, 5);
    MEEP(C4, 5);
    MEEP(A3, 22);

    beep(SHH);
}

void hes_a_pirate()
{
    beep(E3, 3);
    beep(G3, 3);
    beep(A3, 3);
    sleep_ticks(3);
    beep(A3, 7);

    beep(A3, 3);
    beep(B3, 3);
    beep(C4, 3);
    sleep_ticks(3);
    beep(C4, 7);

    beep(C4, 3);
    beep(D4, 3);
    beep(B3, 3);
    sleep_ticks(3);
    beep(B3, 7);

    beep(A3, 3);
    beep(G3, 3);
    beep(A3, 3);

    sleep_ticks(10);

    beep(E3, 3);
    beep(G3, 3);
    beep(A3, 3);
    sleep_ticks(3);
    beep(A3, 7);

    beep(A3, 3);
    beep(B3, 3);
    beep(C4, 3);
    sleep_ticks(3);
    beep(C4, 7);

    beep(C4, 3);
    beep(D4, 3);
    beep(B3, 3);
    sleep_ticks(3);
    beep(B3, 7);

    beep(G3, 3);
    beep(A3, 3);

    sleep_ticks(10);

    beep(E3, 3);
    beep(G3, 3);
    beep(A3, 3);
    sleep_ticks(3);
    beep(A3, 7);

    beep(A3, 3);
    beep(B3, 3);
    beep(C4, 3);
    sleep_ticks(3);
    beep(C4, 7);

    beep(C4, 3);
    beep(D4, 3);
    beep(E4, 3);
    sleep_ticks(3);
    beep(E4, 3);

    beep(D4, 3);
    beep(C4, 3);
    beep(D4, 3);
    beep(A3, 7);

    sleep_ticks(10);

    beep(A3, 3);
    beep(B3, 3);
    beep(C4, 3);
    sleep_ticks(3);
    beep(C4, 7);

    beep(D4, 7);
    beep(E4, 4);
    beep(A3, 7);

    beep(A3, 3);
    beep(C4, 3);
    beep(B3, 18);
    sleep_ticks(3);
    beep(B3, 7);
    beep(C4, 3);
    beep(A3, 3);
    beep(B3, 7);
}

void outer_wilds()
{
    beep(E3, 10);
    beep(A3, 10);
    beep(C4sus, 12);

    beep(A3, 7);
    beep(D4, 7);
    beep(C4sus, 7);
    beep(B3, 7);
    beep(A3, 7);
    beep(B3, 7);
    beep(C4sus, 7);
    beep(A3, 10);

    beep(E3, 10);
    beep(A3, 10);
    beep(C4sus, 12);

    beep(A3, 7);
    beep(D4, 7);
    beep(C4sus, 7);
    beep(B3, 7);
    beep(A3, 7);
    beep(B3, 7);
    beep(C4sus, 7);
    beep(E4, 7);
    beep(C4sus, 7);

    beep(E3, 10);
    beep(A3, 10);
    beep(C4sus, 12);

    beep(A3, 7);
    beep(D4, 7);
    beep(C4sus, 7);
    beep(B3, 7);
    beep(A3, 7);
    beep(B3, 7);
    beep(C4sus, 7);
    beep(A3, 10);

    beep(E3, 10);
    beep(A3, 10);
    beep(C4sus, 12);

    beep(A3, 7);
    beep(D4, 7);
    beep(C4sus, 7);
    beep(B3, 7);
    beep(A3, 7);
    beep(B3, 7);
    beep(C4sus, 7);
    beep(E4, 7);
    beep(C4sus, 7);

    beep(E4, 7);
    beep(C4sus, 7);
    beep(E4, 7);
    beep(C4sus, 7);

    beep(A3, 7);
    beep(C4sus, 7);
    beep(A3, 7);
    beep(C4sus, 7);

    beep(E4, 7);
    beep(C4sus, 7);
    beep(E4, 7);
    beep(C4sus, 7);

    beep(A3, 7);
    beep(C4sus, 7);
    beep(A3, 7);
    beep(C4sus, 7);

    beep(E4, 7);
    beep(C4sus, 7);
    beep(E4, 7);
    beep(C4sus, 7);

    beep(A3, 7);
    beep(C4sus, 7);
    beep(A3, 7);
    beep(C4sus, 7);

    beep(E4, 7);
    beep(C4sus, 7);
    beep(E4, 7);
    beep(C4sus, 7);
}

void do_i_wanna_know()
{
    beep(F3, 3);
    beep(G3, 3);
    beep(A3sus, 8);
    beep(G3, 18);

    sleep_ticks(18);

    beep(F3, 3);
    beep(G3, 3);
    beep(A3sus, 5);
    beep(F4, 6);
    beep(D4sus, 18);
    beep(D4, 6);
    beep(C4, 18);

    sleep_ticks(3);

    beep(A3sus, 3);
    beep(C4, 3);
    beep(D4, 3);
    beep(C4, 9);

    sleep_ticks(6);

    beep(A3sus, 4);
    beep(C4, 4);
    beep(D4, 4);
    sleep_ticks(2);
    beep(D4, 4);
    beep(G3, 18);

    sleep_ticks(9);

    beep(F3, 3);
    beep(G3, 3);
    beep(A3sus, 8);
    beep(G3, 18);
}

void whistle_stop()
{
    beep(A4sus, 6);
    beep(D3, 3);
    beep(G3, 6);
    beep(D3, 3);
    beep(G3, 3);
    beep(C3, 3);
    beep(G3, 9);

    sleep_ticks(9);

    beep(D3sus, 3);
    beep(G3, 3);
    beep(C3, 3);
    beep(G3, 9);

    sleep_ticks(9);

    beep(D3sus, 3);
    beep(G3, 3);
    beep(C3, 3);
    beep(G3, 9);
}
