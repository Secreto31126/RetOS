#include "phylos.h"

void print_state()
{
    unsigned int max = 0;
    char *to_print;
    while (1)
    {
        sem_wait(mutex); // Mutex lock
        for (unsigned int i = 0; i < *phylo_count; i++)
        {
            if (phylos[i].state == EATING)
            {
                puts("E ");
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
        sem_post(mutex); // Mutex unlock
        sleep(1);
    }
}
