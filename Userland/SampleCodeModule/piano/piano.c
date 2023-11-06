#include "./piano.h"

static uint8_t exit = 0;

void startPiano()
{
    blank();
    paintString("Welcome to the piano!\n\nPress 'q' to exit.\n", -1, 0);
    char note;
    while ((note = getChar()) != 'q')
    {
        switch (note)
        {
        case 'z':
            playFor(261, 1);
            note = 0;
            break;
        case 's':
            playFor(277, 1);
            note = 0;
            break;
        case 'x':
            playFor(293, 1);
            note = 0;
            break;
        case 'd':
            playFor(311, 1);
            note = 0;
            break;
        case 'c':
            playFor(329, 1);
            note = 0;
            break;
        case 'v':
            playFor(349, 1);
            note = 0;
            break;
        case 'g':
            playFor(369, 1);
            note = 0;
            break;
        case 'b':
            playFor(392, 1);
            note = 0;
            break;
        case 'h':
            playFor(415, 1);
            note = 0;
            break;
        case 'n':
            playFor(440, 1);
            note = 0;
            break;
        case 'j':
            playFor(466, 1);
            note = 0;
            break;
        case 'm':
            playFor(493, 1);
            note = 0;
            break;
        case '1':
            imperial_march();
            note = 0;
            break;
        case '2':
            hes_a_pirate();
            note = 0;
            break;
        case '3':
            outer_wilds();
            note = 0;
            break;
        case '4':
            do_i_wanna_know();
            note = 0;
            break;
        case '5':
            sports_center();
            note = 0;
            break;
        case '6':
            here_comes_the_sun();
            note = 0;
            break;
        default:
            shut();
            break;
        }
    }
}
