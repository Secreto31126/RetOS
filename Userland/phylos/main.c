/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include "lib/my_lib.h"
#include "phylos.h"

extern char bss;
extern char endOfBinary;

Data *data = NULL;

int children[MAX_PHYLOS + 1];

int add_phylo()
{
	unsigned int num = data->phylo_count;

	sem_wait(data->phylos[num - 1].sem); // only add a philosopher when no one is using the right-most fork
	puts("Here");

	sem_unlink(strandnum("sem_", num));
	data->phylos[num].sem = sem_open(strandnum("sem_", num), 0);
	if (data->phylos[num].sem == NULL)
	{
		puts("Failed to open semaphore\n");
		return 1;
	}

	data->phylos[num].state = THINKING;

	int pid = fork();

	if (pid < 0)
	{
		// fork failed

		puts("Failed to fork");
		sem_unlink(strandnum("sem_", num));
		return 1;
	}

	if (!pid)
	{
		// child process

		sem_wait(data->childex); // wait until father has made room for you
		phylosopher(num);
		exit(0); // shouldn't get here
	}
	// parent process

	children[num] = pid;
	data->phylo_count++;
	sem_post(data->phylos[num - 1].sem); // return the fork
	puts("Here");
	sem_post(data->childex); // tell child it is ready to philosophyze
	return 0;
}

int main(int argc, char *argv[])
{
	puts("Initializing phylos\n");
	data = malloc(sizeof(Data)); // cumple la función de pseudo shm
	if (data == NULL)
	{
		puts("Failed to allocate memory\n");
		return 1;
	}
	sem_unlink("mutex");   // mutex para controlar el acceso a los estados de los filósofos
	sem_unlink("childex"); // mutex para controlar el acceso a los estados de los filósofos
	data->mutex = sem_open("mutex", 1);
	data->childex = sem_open("childex", 0);
	if (data->mutex == NULL || data->childex == NULL)
	{
		puts("Failed to open semaphore\n");
		free(data);
		return 1;
	}
	data->phylo_count = 5;

	for (unsigned int i = 0; i < MAX_PHYLOS; i++)
	{
		data->phylos[i].state = THINKING;
		sem_unlink(strandnum("sem_", i));
		data->phylos[i].sem = sem_open(strandnum("sem_", i), 1);
		if (data->phylos[i].sem == NULL)
		{
			puts("Failed to open semaphore\n");
			leave(i + 1);
			return 1;
		}
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
			phylosopher(i);
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

	char buffer[15] = {0};
	while (1)
	{
		read(STD_IN, buffer, 1);
		switch (buffer[0])
		{
		case 'a':
		{
			puts("adding\n");
			if (data->phylo_count < MAX_PHYLOS)
			{
				add_phylo();
			}
			else
			{
				puts("Limit reached\n");
			}

			break;
		}
		case 'r':
		{
			puts("removing\n");
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
			puts("quitting\n");
			leave(data->phylo_count);
			return 0;
		}
		default:
			break;
		}
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
