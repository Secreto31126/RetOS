#include "phylos.h"

void print_state()
{
    char devNull;
    unsigned int max = 0;
    char *to_print;
    while (1)
    {
        for (unsigned int i = 0; i < *phylo_count; i++)
        {
            if (phylos[i].state == EATING)
            {
                puts("E ");
            }
            else if (phylos[i].state == THINKING)
            {
                puts("T ");
            }
            else
            {
                puts(". ");
            }

            max = max > phylos[i].turn ? max : phylos[i].turn;
        }
        to_print = strandnum("    Max: ", max);
        puts(to_print);
        puts("\n");
        free(to_print);
        sem_post(mutex);           // Mutex unlock
        read(STD_IN, &devNull, 1); // dupped to a write pipe on phylos, gets written to whenever state updates. Before this write, mutex is waited on
    }
}
