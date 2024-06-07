#include "phylos.h"

void print_state()
{
    while (1)
    {
        sem_wait(data->mutex); // Mutex lock
        for (unsigned int i = 0; i < data->phylo_count; i++)
        {
            if (data->phylos[i].state == EATING)
            {
                puts("E ");
            }
            else
            {
                puts(". ");
            }
        }
        puts("\n");
        sem_post(data->mutex); // Mutex unlock
        sleep(1);
    }
}
