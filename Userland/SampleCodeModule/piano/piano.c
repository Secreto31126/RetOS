#include "./piano.h"

static uint8_t exit = 0;
static Songs *songs;

void addSongs(Songs *songs);

void startPiano()
{
    if ((songs = malloc(sizeof(Songs) * 6)) != 0)
    {
        addSongs(songs);
    }
    else
    {
        paintString("Error allocating memory for songs.\n", -1, 0);
        return;
    }
    // PianoNote * notes = malloc(sizeof(PianoNote) * 12);
    blank();
    paintString("Welcome to the piano!\n\nPress 'q' to exit.\n", -1, 0);
    char note;
    while ((note = readChar()) != 'q')
    {
        switch (note)
        {
        case 'z':
            play(261);
            // paintString("Do\n", -1, 0);
            break;
        case 's':
            play(277);
            // paintString("Do#\n", -1, 0);
            break;
        case 'x':
            play(293);
            // paintString("Re\n", -1, 0);
            break;
        case 'd':
            play(311);
            // paintString("Re#\n", -1, 0);
            break;
        case 'c':
            play(329);
            // paintString("Mi\n", -1, 0);
            break;
        case 'v':
            play(349);
            // paintString("Fa\n", -1, 0);
            break;
        case 'g':
            play(369);
            // paintString("Fa#\n", -1, 0);
            break;
        case 'b':
            play(392);
            // paintString("Sol\n", -1, 0);
            break;
        case 'h':
            play(415);
            // paintString("Sol#\n", -1, 0);
            break;
        case 'n':
            play(440);
            // paintString("La\n", -1, 0);
            break;
        case 'j':
            play(466);
            // paintString("La#\n", -1, 0);
            break;
        case 'm':
            play(493);
            // paintString("Si\n", -1, 0);
            break;
        case ',':
            play(523);
            // paintString("Do\n", -1, 0);
            break;
        default:
            if (note > '0' && note <= '6')
            {
                unsigned char index = note - '1';
                paintString(songs[index].name, -1, 0);
                songs[index].song();
            }
            shut();
            break;
        }

        char tmp = note;
        while (tmp == (note = readChar()) && note != 0)
            halt_user();

        halt_user();
    }
    free(songs);
    blank();
}

void addSongs(Songs *songs)
{
    songs[0].song = imperial_march;
    songs[0].name = "Imperial March\n";
    songs[1].song = hes_a_pirate;
    songs[1].name = "He's a Pirate\n";
    songs[2].song = outer_wilds;
    songs[2].name = "Outer Wilds\n";
    songs[3].song = do_i_wanna_know;
    songs[3].name = "Do I Wanna Know\n";
    songs[4].song = sports_center;
    songs[4].name = "Sports Center\n";
    songs[5].song = here_comes_the_sun;
    songs[5].name = "Here Comes the Sun\n";
}
