#include "./piano.h"

static uint8_t exit = 0;

void paintStringOrWarp(char *s);

void startPiano()
{
    char note;
    while ((note = getchar()) != 'q')
    {
        switch (note)
        {
        case 'z':
            play(261);
            break;
        case 's':
            play(277);
            break;
        case 'x':
            play(293);
            break;
        case 'd':
            play(311);
            break;
        case 'c':
            play(329);
            break;
        case 'v':
            play(349);
            break;
        case 'g':
            play(369);
            break;
        case 'b':
            play(392);
            break;
        case 'h':
            play(415);
            break;
        case 'n':
            play(440);
            break;
        case 'j':
            play(466);
            break;
        case 'm':
            play(493);
            break;
        case '1':
            imperialMarch();
            break;
        case '2':
            hes_a_pirate();
            break;
        case '3':
            outer_wilds();
            break;
        case '4':
            do_i_wanna_know();
            break;
        case '5':
            whistle_stop();
            break;
        case '6':
            here_comes_the_sun();
            break;
        default:
            shut();
            break;
        }
    }
}
