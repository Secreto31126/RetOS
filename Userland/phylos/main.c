/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include "lib/my_lib.h"
#include "phylos.h"

extern char bss;
extern char endOfBinary;

#define puts(str) write(1, (str), strlen(str))

sem_t *mutex;
sem_t **sems;
int *states;

int children[MAX_PHYLOS + 1];

int main(int argc, char *argv[])
{
	puts("Initializing phylos\n");
	mutex = sem_open("mutex", 1);
	states = malloc(MAX_PHYLOS * sizeof(int));
	sems = malloc(MAX_PHYLOS * sizeof(sem_t *));

	for (int i = 0; i < 3; i++)
	{
		states[i] = THINKING;
		sems[i] = sem_open(strandnum("sem", i), 0);
		if (sems[i] == NULL)
		{
			puts("Failed to open semaphore\n");
			return 1;
		}
	}

	int pid;
	for (int i = 0; i < 3; i++)
	{
		pid = fork();
		if (pid < 0)
		{
			puts("Failed to fork");
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

	puts("I'm the manager\n");
	states[1] = HUNGRY;

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

	for (int i = 0; i < 3; i++)
	{
		waitpid(children[i], NULL, 0);
	}
	waitpid(-1, NULL, 0);
	puts("Exiting phylos\n");
	free(states);
	sem_close(mutex);

	exit(0);
}

void phylosopher(int i)
{
	char *presentation = strandnum("I'm phylosopher ", i);
	puts(presentation);
	puts("\n");
	free(presentation);
	while (1)
	{
		take_forks(i);
		sleep(1);
		put_forks(i);
		sleep(1);
	}
}

void print_state()
{
	char *to_print;
	while (1)
	{
		sem_wait(mutex); // Mutex lock
		for (int i = 0; i < 3; i++)
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
		puts("\n");
		sem_post(mutex); // Mutex unlock
		sleep(1);
	}
}

void take_forks(int i)
{
	sem_wait(mutex); // Mutex lock
	states[i] = HUNGRY;
	test(i);
	sem_post(mutex);   // Mutex unlock
	sem_wait(sems[i]); // Wait until able to eat
}

void put_forks(int i)
{
	sem_wait(mutex); // Mutex lock
	states[i] = THINKING;
	test(RIGHT(i, 3));
	test(LEFT(i, 3));
	sem_post(mutex); // Mutex unlock
}

void test(int i)
{
	if (states[i] == HUNGRY && states[LEFT(i, 3)] != EATING && states[RIGHT(i, 3)] != EATING)
	{
		states[i] = EATING;
		sem_post(sems[i]);
	}
}
