#include "./piano.h"

static uint8_t exit = 0;
static Songs *songs;

void paintStringOrWarp(char *s);

void startPiano()
{
    blank();
    if ((songs = malloc(sizeof(Songs) * 6)) != 0)
    {
        addSongs();
    }
    else
    {
        paintStringOrWarp("Error allocating memory for songs.\n");
        return;
    }
    paintStringOrWarp("Welcome to the piano!\n\n");
    paintStringOrWarp("Press 'q' to exit.\n");
    // PianoNote * notes = malloc(sizeof(PianoNote) * 12);
    char note;
    while ((note = readChar()) != 'q')
    {
        switch (note)
        {
        case 'z':
            play(261);
            paintStringOrWarp("Do\n");
            break;
        case 's':
            play(277);
            paintStringOrWarp("Do#\n");
            break;
        case 'x':
            play(293);
            paintStringOrWarp("Re\n");
            break;
        case 'd':
            play(311);
            paintStringOrWarp("Re#\n");
            break;
        case 'c':
            play(329);
            paintStringOrWarp("Mi\n");
            break;
        case 'v':
            play(349);
            paintStringOrWarp("Fa\n");
            break;
        case 'g':
            play(369);
            paintStringOrWarp("Fa#\n");
            break;
        case 'b':
            play(392);
            paintStringOrWarp("Sol\n");
            break;
        case 'h':
            play(415);
            paintStringOrWarp("Sol#\n");
            break;
        case 'n':
            play(440);
            paintStringOrWarp("La\n");
            break;
        case 'j':
            play(466);
            paintStringOrWarp("La#\n");
            break;
        case 'm':
            play(493);
            paintStringOrWarp("Si\n");
            break;
        case ',':
            play(523);
            paintStringOrWarp("Do\n");
            break;
        default:
            if (note > '0' && note <= '6')
            {
                char index = note - '1';
                songs[index].song();
                paintStringOrWarp(songs[index].name);
            }
            shut();
            break;
        }
        while ((note == readChar()) && (note != 0))
        {
            halt_user();
        }
        shut();
    }
    free(songs);
    blank();
}

void addSongs()
{
    songs[0].song = imperial_march;
    songs[0].name = "Imperial March";
    songs[1].song = hes_a_pirate;
    songs[1].name = "He's a Pirate";
    songs[2].song = outer_wilds;
    songs[2].name = "Outer Wilds";
    songs[3].song = do_i_wanna_know;
    songs[3].name = "Do I Wanna Know";
    songs[4].song = sports_center;
    songs[4].name = "Sports Center";
    songs[5].song = here_comes_the_sun;
    songs[5].name = "Here Comes the Sun";
}
