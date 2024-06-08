#include "phylos.h"

void print_state()
{
    char devNull;
    while (1)
    {
        sem_wait(data->printex);
        for (unsigned int i = 0; i < data->phylo_count; i++)
        {
            if (data->phylos[i].state == EATING)
            {
                puts("E ");
            }
            else if (data->phylos[i].state == THINKING)
            {
                puts("T ");
            }
            else
            {
                puts(". ");
            }
        }
        puts("\n");
        sem_post(data->mutex); // Mutex unlock, should be locked by phylos before writing to this pipe
    }
}
