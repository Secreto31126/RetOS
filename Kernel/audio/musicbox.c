/**
 * @brief A cool box that plays music
 * @deprecated It was fun while it lasted
 */

// #include "audio.h"

// #define SHH 0
// #define C3 262
// #define C3sus 278
// #define D3 294
// #define D3sus 312
// #define E3 330
// #define F3 350
// #define F3sus 370
// #define G3 392
// #define G3sus 416
// #define A3 440
// #define A3sus 466
// #define B3 494
// #define C4 524
// #define C4sus 554
// #define D4 588
// #define D4sus 622
// #define E4 660
// #define F4 698
// #define F4sus 740
// #define G4 784
// #define G4sus 830
// #define A4 880
// #define A4sus 932
// #define B4 988
// #define C5 1046

// GCC cries if I comment you...
// I guess I can keep you around...
// Just promise you won't bring any issues, okay?
#define MEEP(f, d)                                                             \
  beep(f);                                                                     \
  sleep_ticks(d);

// /**
//  * @see https://github.com/gabszzz/The-Imperial-March
//  */
// void imperial_march()
// {
//     MEEP(A3, 14);
//     MEEP(SHH, 3);
//     MEEP(A3, 14);
//     MEEP(SHH, 3);
//     MEEP(A3, 14);
//     MEEP(SHH, 10);
//     MEEP(F3, 5);
//     MEEP(C4, 4);
//     MEEP(A3, 14);
//     MEEP(F3, 5);
//     MEEP(C4, 4);
//     MEEP(A3, 22);
//     MEEP(SHH, 10);
//     MEEP(E4, 12);
//     MEEP(E4, 12);
//     MEEP(E4, 12);
//     MEEP(SHH, 10);
//     MEEP(F4, 6);
//     MEEP(D4, 5);
//     MEEP(A3, 12);
//     MEEP(F3, 5);
//     MEEP(C4, 5);
//     MEEP(A3, 22);

//     beep(SHH);
// }

// void hes_a_pirate()
// {
//     MEEP(E3, 3);
//     MEEP(G3, 3);
//     MEEP(A3, 3);
//     MEEP(SHH, 3);
//     MEEP(A3, 7);

//     MEEP(A3, 3);
//     MEEP(B3, 3);
//     MEEP(C4, 3);
//     MEEP(SHH, 3);
//     MEEP(C4, 7);

//     MEEP(C4, 3);
//     MEEP(D4, 3);
//     MEEP(B3, 3);
//     MEEP(SHH, 3);
//     MEEP(B3, 7);

//     MEEP(A3, 3);
//     MEEP(G3, 3);
//     MEEP(A3, 3);

//     MEEP(SHH, 10);

//     MEEP(E3, 3);
//     MEEP(G3, 3);
//     MEEP(A3, 3);
//     MEEP(SHH, 3);
//     MEEP(A3, 7);

//     MEEP(A3, 3);
//     MEEP(B3, 3);
//     MEEP(C4, 3);
//     MEEP(SHH, 3);
//     MEEP(C4, 7);

//     MEEP(C4, 3);
//     MEEP(D4, 3);
//     MEEP(B3, 3);
//     MEEP(SHH, 3);
//     MEEP(B3, 7);

//     MEEP(G3, 3);
//     MEEP(A3, 3);

//     MEEP(SHH, 10);

//     MEEP(E3, 3);
//     MEEP(G3, 3);
//     MEEP(A3, 3);
//     MEEP(SHH, 3);
//     MEEP(A3, 7);

//     MEEP(A3, 3);
//     MEEP(B3, 3);
//     MEEP(C4, 3);
//     MEEP(SHH, 3);
//     MEEP(C4, 7);

//     MEEP(C4, 3);
//     MEEP(D4, 3);
//     MEEP(E4, 3);
//     MEEP(SHH, 3);
//     MEEP(E4, 3);

//     MEEP(D4, 3);
//     MEEP(C4, 3);
//     MEEP(D4, 3);
//     MEEP(A3, 7);

//     MEEP(SHH, 10);

//     MEEP(A3, 3);
//     MEEP(B3, 3);
//     MEEP(C4, 3);
//     MEEP(SHH, 3);
//     MEEP(C4, 7);

//     MEEP(D4, 7);
//     MEEP(E4, 4);
//     MEEP(A3, 7);

//     MEEP(A3, 3);
//     MEEP(C4, 3);
//     MEEP(B3, 18);
//     MEEP(SHH, 3);
//     MEEP(B3, 7);
//     MEEP(C4, 3);
//     MEEP(A3, 3);
//     MEEP(B3, 7);

//     beep(SHH);
// }

// void outer_wilds()
// {
//     MEEP(E3, 10);
//     MEEP(A3, 10);
//     MEEP(C4sus, 12);

//     MEEP(A3, 7);
//     MEEP(D4, 7);
//     MEEP(C4sus, 7);
//     MEEP(B3, 7);
//     MEEP(A3, 7);
//     MEEP(B3, 7);
//     MEEP(C4sus, 7);
//     MEEP(A3, 10);

//     MEEP(E3, 10);
//     MEEP(A3, 10);
//     MEEP(C4sus, 12);

//     MEEP(A3, 7);
//     MEEP(D4, 7);
//     MEEP(C4sus, 7);
//     MEEP(B3, 7);
//     MEEP(A3, 7);
//     MEEP(B3, 7);
//     MEEP(C4sus, 7);
//     MEEP(E4, 7);
//     MEEP(C4sus, 7);

//     MEEP(E3, 10);
//     MEEP(A3, 10);
//     MEEP(C4sus, 12);

//     MEEP(A3, 7);
//     MEEP(D4, 7);
//     MEEP(C4sus, 7);
//     MEEP(B3, 7);
//     MEEP(A3, 7);
//     MEEP(B3, 7);
//     MEEP(C4sus, 7);
//     MEEP(A3, 10);

//     MEEP(E3, 10);
//     MEEP(A3, 10);
//     MEEP(C4sus, 12);

//     MEEP(A3, 7);
//     MEEP(D4, 7);
//     MEEP(C4sus, 7);
//     MEEP(B3, 7);
//     MEEP(A3, 7);
//     MEEP(B3, 7);
//     MEEP(C4sus, 7);
//     MEEP(E4, 7);
//     MEEP(C4sus, 7);

//     MEEP(E4, 7);
//     MEEP(C4sus, 7);
//     MEEP(E4, 7);
//     MEEP(C4sus, 7);

//     MEEP(A3, 7);
//     MEEP(C4sus, 7);
//     MEEP(A3, 7);
//     MEEP(C4sus, 7);

//     MEEP(E4, 7);
//     MEEP(C4sus, 7);
//     MEEP(E4, 7);
//     MEEP(C4sus, 7);

//     MEEP(A3, 7);
//     MEEP(C4sus, 7);
//     MEEP(A3, 7);
//     MEEP(C4sus, 7);

//     MEEP(E4, 7);
//     MEEP(C4sus, 7);
//     MEEP(E4, 7);
//     MEEP(C4sus, 7);

//     MEEP(A3, 7);
//     MEEP(C4sus, 7);
//     MEEP(A3, 7);
//     MEEP(C4sus, 7);

//     MEEP(E4, 7);
//     MEEP(C4sus, 7);
//     MEEP(E4, 7);
//     MEEP(C4sus, 7);

//     beep(SHH);
// }

// void do_i_wanna_know()
// {
//     MEEP(F3, 3);
//     MEEP(G3, 3);
//     MEEP(A3sus, 8);
//     MEEP(G3, 18);

//     MEEP(SHH, 18);

//     MEEP(F3, 3);
//     MEEP(G3, 3);
//     MEEP(A3sus, 5);
//     MEEP(F4, 6);
//     MEEP(D4sus, 18);
//     MEEP(D4, 6);
//     MEEP(C4, 18);

//     MEEP(SHH, 3);

//     MEEP(A3sus, 3);
//     MEEP(C4, 3);
//     MEEP(D4, 3);
//     MEEP(C4, 9);

//     MEEP(SHH, 6);

//     MEEP(A3sus, 4);
//     MEEP(C4, 4);
//     MEEP(D4, 4);
//     MEEP(SHH, 2);
//     MEEP(D4, 4);
//     MEEP(G3, 18);

//     MEEP(SHH, 9);

//     MEEP(F3, 3);
//     MEEP(G3, 3);
//     MEEP(A3sus, 8);
//     MEEP(G3, 18);

//     beep(SHH);
// }

// void whistle_stop()
// {
//     MEEP(A4sus, 6);
//     MEEP(D3, 3);
//     MEEP(G3, 6);
//     MEEP(D3, 3);
//     MEEP(G3, 3);
//     MEEP(C3, 3);
//     MEEP(G3, 9);

//     MEEP(SHH, 9);

//     MEEP(D3sus, 3);
//     MEEP(G3, 3);
//     MEEP(C3, 3);
//     MEEP(G3, 9);

//     MEEP(SHH, 9);

//     MEEP(D3sus, 3);
//     MEEP(G3, 3);
//     MEEP(C3, 3);
//     MEEP(G3, 9);

//     beep(SHH);
// }

// void here_comes_the_sun()
// {
//     MEEP(B4, 9);
//     MEEP(A4, 9);
//     MEEP(B4, 9);
//     MEEP(G4, 9);

//     MEEP(SHH, 7);

//     MEEP(B4, 7);
//     MEEP(G4, 9);
//     MEEP(A4, 9);
//     MEEP(B4, 9);

//     MEEP(SHH, 2);

//     MEEP(B4, 9);
//     MEEP(A4, 9);
//     MEEP(B4, 9);
//     MEEP(G4, 12);

//     MEEP(SHH, 2);

//     MEEP(G4, 9);
//     MEEP(A4, 9);
//     MEEP(G4, 9);

//     MEEP(SHH, 9);

//     MEEP(B4, 9);
//     MEEP(A4, 9);
//     MEEP(G4, 9);

//     beep(SHH);
// }
