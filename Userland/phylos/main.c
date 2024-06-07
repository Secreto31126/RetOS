/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include "lib/my_lib.h"
#include "phylos.h"

extern char bss;
extern char endOfBinary;

Data *data = NULL;

int children[MAX_PHYLOS + 1];

int main(int argc, char *argv[])
{
	puts("Initializing phylos\n");
	data = malloc(sizeof(Data));
	if (data == NULL)
	{
		puts("Failed to allocate memory\n");
		return 1;
	}
	data->mutex = sem_open("mutex", 0);
	if (data->mutex == NULL)
	{
		puts("Failed to open semaphore\n");
		free(data);
		return 1;
	}
	data->phylo_count = 5;

	for (unsigned int i = 0; i < MAX_PHYLOS; i++)
	{
		data->phylos[i].state = THINKING;
	}

	int pid;
	for (unsigned int i = 0; i < data->phylo_count; i++)
	{
		pid = fork();
		if (pid < 0)
		{
			puts("Failed to fork");
			leave(i);
			return 1;
		}
		else if (pid == 0)
		{
			sem_unlink(strandnum("sem_", i));
			data->phylos[i].sem = sem_open(strandnum("sem_", i), 1);
			if (data->phylos[i].sem == NULL)
			{
				puts("Failed to open semaphore\n");
				leave(i + 1);
				return 1;
			}
			return 0;
		}
		else
		{
			children[i] = pid;
		}
	}

	pid = fork();
	if (pid < 0)
	{
		puts("Failed to fork");
		return 1;
	}
	else if (pid == 0)
	{
		print_state();
		return 0;
	}
	else
	{
		children[MAX_PHYLOS] = pid;
	}

	for (int i = 0; i < data->phylo_count; i++)
	{
		phylosopher(i);
	}
	sem_post(data->mutex);
	char buffer[15] = {0};
	while (1)
	{
		read(STD_IN, buffer, 1);
		sem_wait(data->mutex);
		switch (buffer[0])
		{
		case 'a':
		{
			puts("dding\n");
			if (data->phylo_count < MAX_PHYLOS)
			{
				pid = fork();
				unsigned int num = data->phylo_count;
				if (pid < 0)
				{
					puts("Failed to fork");
				}
				else if (pid == 0)
				{
					data->phylos[num].state = THINKING;
					sem_unlink(strandnum("sem_", num));
					data->phylos[num].sem = sem_open(strandnum("sem_", num), 0);
					if (data->phylos[num].sem == NULL)
					{
						puts("Failed to open semaphore\n");
						leave(num + 1);
						break;
					}
					phylosopher(num);
					return 0;
				}
				else
				{
					children[num] = pid;
					data->phylo_count = data->phylo_count + 1;
				}
			}
			else
			{
				puts("Limit reached\n");
			}

			break;
		}
		case 'r':
		{
			puts("emoving\n");
			data->phylo_count = data->phylo_count - 1;
			sem_close(data->phylos[data->phylo_count].sem);
			kill(children[data->phylo_count], SIGKILL);
			waitpid(children[data->phylo_count], NULL, 0);
			if (data->phylo_count <= 0)
			{
				puts("No more phylos\n");
				leave(0);
				return 0;
			}

			break;
		}
		case 'q':
		{
			puts("uiting\n");
			leave(data->phylo_count);
			return 0;
		}
		default:
			break;
		}

		sem_post(data->mutex);
	}

	return 0;
}

void leave(int count)
{
	for (unsigned int i = 0; i < count; i++)
	{
		sem_close(data->phylos[i].sem);
		kill(children[i], SIGKILL);
		waitpid(children[i], NULL, 0);
	}
	kill(children[MAX_PHYLOS], SIGKILL);
	waitpid(children[MAX_PHYLOS], NULL, 0);
	sem_close(data->mutex);
	free(data);
	puts("byee!\n");
}
