#include "phylos.h"

void print_state()
{
    while (1)
    {
        sem_wait(data->printex[1]); // printex lock, should be posted by phylos when a state changes
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
            else if (data->phylos[i].state == HUNGRY)
            {
                puts(". ");
            }
            else
            {
                char c[] = {data->phylos[i].state, 0};
                puts(c);
            }
        }
        puts("\n");
        sem_post(data->printex[0]); // printex unlock, should be waited by phylos when a state changes
    }
}
