/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include "lib/my_lib.h"
#include "phylos.h"
#include "random.h"

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

int add_philo(int use_addex)
{
	int i = data->phylo_count;

	if (i && use_addex)
	{
		sem_wait(data->mutex); // acquire mutex

		data->adding = i - 1;  // notify last philo you will add a new one
		sem_wait(data->addex); // wait for philo that must be notified to be in right state
	}
	data->phylos[i].state = THINKING; // All philos start in THINKING state
	if (make_sem(i) || make_philo(i))
	{
		if (i && use_addex)
		{
			data->adding = -1;
			sem_post(data->mutex);
		}
		return 1;
	}

	(data->phylo_count)++;
	if (i)
	{
		data->adding = -1;
		sem_post(data->mutex);
	}
	sem_post(data->childex); // wake up new philo

	return 0;
}

int make_mutexes()
{
	sem_unlink("mutex");   // mutex para controlar el acceso a los estados de los filósofos
	sem_unlink("childex"); // mutex para controlar la inicialización de un nuevo filósofo
	sem_unlink("addex");   // mutex para controlar el acceso al último tenedor al añadir un filosofo
	sem_unlink("printex0");
	sem_unlink("printex1");

	data->mutex = sem_open("mutex", 1);
	data->childex = sem_open("childex", 0);
	data->addex = sem_open("addex", 0);
	data->printex[0] = sem_open("printex0", 1);
	data->printex[1] = sem_open("printex1", 0);

	if (data->mutex == NULL || data->childex == NULL || data->addex == NULL || data->printex[0] == NULL || data->printex[1] == NULL)
		return 1;
	return 0;
}

int main(int argc, char *argv[])
{
	puts("Initializing phylos\n");

	setSeed(get_tick()); // This is questionable, but we aren't cryptographers

	data = malloc(sizeof(Data)); // cumple la función de pseudo shm
	data->printex = malloc(sizeof(sem_t *) * 2);
	if (data == NULL || data->printex == NULL)
	{
		puts("Failed to allocate memory\n");
		return 1;
	}

	if (make_mutexes())
	{
		puts("Failed to open semaphore\n");
		free(data);
		return 1;
	}

	data->phylo_count = 0;

	for (unsigned int i = 0; i < INITIAL_PHYLOS; i++)
	{
		if (add_philo(0))
		{
			leave(i);
			return 1;
		}
	}
	for (unsigned int i = 0; i < INITIAL_PHYLOS; i++)
	{
		sem_post(data->childex); // wake up the philos all at once
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
				add_philo(1);
				sem_post(data->childex); // wake up the philo
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
			(data->phylo_count)--;
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
	leave(data->phylo_count);
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
	sem_close(data->printex);
	free(data->printex);
	free(data);
	puts("byee!\n");
}
