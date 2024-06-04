/* main.c */

#include <sys.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphores.h>
#include "phylos.h"

extern char bss;
extern char endOfBinary;

#define puts(str) write(1, (str), strlen(str))

char *strandnum(const char *str, int num);

sem_t **sems;
sem_t *mutex;
int *states;
int *turns;
int *phylos;

int child_ids[MAX_PHYLOS + 1];

int main(int argc, char *argv[])
{
	int pid;

	sems = malloc(MAX_PHYLOS * sizeof(sem_t *));
	states = malloc(MAX_PHYLOS * sizeof(int));
	turns = malloc(MAX_PHYLOS * sizeof(int));
	phylos = malloc(sizeof(int));
	phylos[0] = 5;

	// Crear mutex
	mutex = sem_open("mutex", 0);

	// Crear procesos de los filósofos
	for (int i = 0; i < phylos[0]; i++)
	{
		pid = fork();
		if (pid < 0)
		{
			puts("Error in fork\n");
			return 1;
		}
		else if (pid == 0)
		{
			states[i] = THINKING;
			turns[i] = 0;
			char *name = strandnum("sem_", i);
			sem_t *sem = sem_open(name, 0);
			free(name);
			if (sem)
			{
				sems[i] = sem;
			}
			else
			{
				puts("Error in sem_open\n");
				return 1;
			}
			philosopher(i);
			exit(0);
		}
		else
		{
			child_ids[i] = pid;
		}
	}

	sem_post(mutex);

	// Crear proceso de impresión
	pid = fork();
	if (pid < 0)
	{
		puts("Error in fork\n");
		return 1;
	}
	else if (pid == 0)
	{
		puts("a: to add phylosopher\nr: Remove phylosopher\nq: Quit\n\n");
		print_state();
		exit(0);
	}
	child_ids[MAX_PHYLOS] = pid;

	char buffer[1];
	int run = 1;
	while (run)
	{
		read(STD_IN, buffer, 1);
		switch ((char)buffer[0])
		{
		case 'a':
		{
			if (phylos[0] < MAX_PHYLOS)
			{
				int j = fork();
				if (j < 0)
				{
					puts("Error in fork\n");
				}
				else if (j == 0)
				{
					states[phylos[0]] = THINKING;
					turns[phylos[0]] = 0;
					char *name = strandnum("sem_", j);
					sem_t *sem = sem_open(name, 0);
					free(name);
					if (sem)
					{
						sems[phylos[0]] = sem;
					}
					else
					{
						puts("Error in sem_open\n");
						return 1;
					}
					philosopher(phylos[0]);
					exit(0);
				}
				else
				{
					child_ids[phylos[0]++] = j;
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
			if (phylos[0] > 0)
			{
				kill(child_ids[--phylos[0]], SIGKILL);
				sem_close(sems[phylos[0]]);
				if (phylos[0] == 0)
				{
					kill(child_ids[MAX_PHYLOS], SIGKILL);
					run = 0;
				}
			}
			break;
		}

		case 'q':
		{
			for (int i = 0; i < phylos[0]; i++)
			{
				sem_close(sems[i]);
				kill(child_ids[i], SIGKILL);
			}
			kill(child_ids[MAX_PHYLOS], SIGKILL);
			run = 0;
			break;
		}

		default:
			break;
		}
	}

	for (int i = 0; i < phylos[0]; i++)
	{
		waitpid(child_ids[i], NULL, 0);
	}
	waitpid(child_ids[MAX_PHYLOS], NULL, 0);

	free(sems);
	free(states);
	free(turns);
	free(phylos);

	sem_close(mutex);

	return 0;
}

char *strandnum(const char *str, int num)
{
	char *buffer = malloc(strlen(str) + 3);
	strcpy(buffer, str);
	itoa(num, buffer + strlen(str), 10);
	return buffer;
}

void print_state()
{
	int max = 0;
	char *to_print;
	while (1)
	{
		to_print = strandnum("Cantidad de filósofos: ", phylos[0]);
		puts(to_print);
		puts("\n");
		free(to_print);
		sem_wait(mutex); // Mutex lock
		for (int i = 0; i < phylos[0]; i++)
		{
			if (states[i] == EATING)
			{
				puts("E ");
			}
			else
			{
				puts(". ");
			}
		}
		for (int i = 0; i < phylos[0]; i++)
		{
			max = turns[i] > max ? turns[i] : max;
		}
		to_print = strandnum("       Max: ", max);
		puts(to_print);
		puts("\n\n");
		free(to_print);
		sem_post(mutex); // Mutex unlock
		sleep(1);
	}
}

void philosopher(int i)
{
	while (1)
	{
		take_forks(i);
		sleep(1); // Tiempo de comer
		put_forks(i);
		sleep(1); // Tiempo de pensar
	}
}

void take_forks(int i)
{
	sem_wait(mutex); // Mutex lock
	states[i] = HUNGRY;
	test(i);
	if (states[i] == EATING)
	{
		turns[i] = 0;
	}
	else
	{
		turns[i]++;
		// if(turns[i] > 5) {
		//     sem_post(mutex); // Mutex unlock
		//     sem_wait(LEFT(i, phylos[0]));
		//     sem_wait(RIGHT(i, phylos[0]));
		//     take_forks(i);
		//     sem_post(RIGHT(i, phylos[0]));
		//     sem_post(LEFT(i, phylos[0]));
		// }
	}
	sem_post(mutex);   // Mutex unlock
	sem_wait(sems[i]); // Wait until able to eat
}

void put_forks(int i)
{
	sem_wait(mutex); // Mutex lock
	states[i] = THINKING;
	test(RIGHT(i, phylos[0]));
	test(LEFT(i, phylos[0]));
	sem_post(mutex); // Mutex unlock
}

void test(int i)
{
	if (states[i] == HUNGRY && states[LEFT(i, phylos[0])] != EATING && states[RIGHT(i, phylos[0])] != EATING)
	{
		states[i] = EATING;
		sem_post(sems[i]);
	}
}
