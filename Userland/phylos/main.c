/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include "lib/my_lib.h"
#include "phylos.h"

extern char bss;
extern char endOfBinary;

Data *data = NULL;

int children[MAX_PHYLOS + 1];

int make_printer()
{
	int pid = fork();
	if (pid < 0)
	{
		puts("Failed to fork");
		return 1;
	}
	else if (pid == 0)
	{
		// child process
		print_state();
		exit(1);
	}
	children[MAX_PHYLOS] = pid;
	return 0;
}

int make_sem(int i)
{
	data->phylos[i].state = THINKING;
	sem_unlink(strandnum("sem_", i));
	data->phylos[i].sem = sem_open(strandnum("sem_", i), 1);
	if (data->phylos[i].sem == NULL)
	{
		puts("Failed to open semaphore\n");
		return 1;
	}
	return 0;
}

void hold_forks(int i)
{
	if (i <= 0)
		return;

	sem_wait(data->phylos[0].sem); // take the leftmost fork

	if (i > 1)
		sem_wait(data->phylos[i - 1].sem); // if the rightmost fork isn't also the leftmost fork, take it as well
}

void return_forks(int i)
{
	if (i <= 0)
		return;

	sem_post(data->phylos[0].sem); // return the leftmost fork

	if (i > 1)
		sem_post(data->phylos[i - 1].sem); // if the rightmost fork isn't also the leftmost fork, return it as well
}

int make_philo(int i)
{

	int pid = fork();
	if (pid < 0)
	{
		puts("Failed to fork");
		return 1;
	}
	else if (!pid)
	{
		// child process
		sem_wait(data->childex);
		phylosopher(i);
		exit(1);
	}
	// parent process
	children[i] = pid;
	return 0;
}

int add_philo()
{
	int i = data->phylo_count;

	hold_forks(i);

	data->phylos[i].state = THINKING; // All philos start in THINKING state
	if (make_sem(i) || make_philo(i))
	{
		return_forks(i);
		sem_post(data->childex);
		return 1;
	}

	(data->phylo_count)++;
	return_forks(i);
	sem_post(data->childex);

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

	data->phylo_count = 0;

	for (unsigned int i = 0; i < INITIAL_PHYLOS; i++)
	{
		if (add_philo())
		{
			leave(i);
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

	if (make_printer())
		return 1;

	char buffer[15] = {0};
	while (1)
	{
		read(STD_IN, buffer, 1);
		switch (buffer[0])
		{
		case 'a':
		case 'A':
		{
			puts("adding\n");
			if (data->phylo_count < MAX_PHYLOS)
			{
				add_philo();
			}
			else
			{
				puts("Limit reached\n");
			}

			break;
		}
		case 'r':
		case 'R':
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
		case 'Q':
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
