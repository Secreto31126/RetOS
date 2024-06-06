/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include "lib/my_lib.h"
#include "phylos.h"
#include "random.h"

extern char bss;
extern char endOfBinary;

sem_t *mutex = NULL;
phylo_t *phylos = NULL;
unsigned int *phylo_count = NULL;

int children[MAX_PHYLOS + 1];

int main(int argc, char *argv[])
{
	setSeed((unsigned int)"Phylos"); // This is questionable, but we aren't cryptographers

	puts("Initializing phylos\n");
	int pipeFd[2]; // For allowing phylos to write to printer
	if (pipe(pipeFd))
	{
		puts("Failed to create pipe\n");
		return 1;
	}
	mutex = sem_open("mutex", 0);
	phylo_t *phylos = malloc(MAX_PHYLOS * sizeof(phylo_t));
	if (phylos == NULL)
	{
		puts("Failed to allocate memory\n");
		return 1;
	}
	phylo_count = malloc(sizeof(unsigned int));
	if (phylo_count == NULL)
	{
		puts("Failed to allocate memory\n");
		return 1;
	}
	*phylo_count = 3;

	for (unsigned int i = 0; i < MAX_PHYLOS; i++)
	{
		phylos[i].state = THINKING;
		phylos[i].turn = 0;
	}

	int pid;
	for (unsigned int i = 0; i < *phylo_count; i++)
	{
		pid = fork();
		if (pid < 0)
		{
			puts("Failed to fork");
			return 1;
		}
		else if (pid == 0)
		{
			close(pipeFd[READ_END]);
			if (dup2(pipeFd[WRITE_END], STD_OUT) < 0)
			{
				puts("Failed to dup2\n");
				return 1;
			}
			phylos[i].sem = sem_open(strandnum("sem_", i), 0);
			if (phylos[i].sem == NULL)
			{
				puts("Failed to open semaphore\n");
				return 1;
			}
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
		close(pipeFd[WRITE_END]);
		if (dup2(pipeFd[READ_END], STD_IN) < 0)
		{
			puts("Failed to dup2\n");
			return 1;
		}
		print_state();
		return 0;
	}
	else
	{
		close(pipeFd[READ_END]);
		children[MAX_PHYLOS] = pid;
	}

	puts("I'm the manager\n");
	sem_post(mutex);
	char buffer[15] = {0};
	while (1)
	{
		read(STD_IN, buffer, 1);
		sem_wait(mutex);
		switch (buffer[0])
		{
		case 'a':
		{
			puts("adding\n");
			if (*phylo_count < MAX_PHYLOS)
			{
				pid = fork();
				unsigned int num = *phylo_count;
				if (pid < 0)
				{
					puts("Failed to fork");
				}
				else if (pid == 0)
				{
					if (dup2(pipeFd[WRITE_END], STD_OUT) < 0)
					{
						puts("Failed to dup2\n");
						return 1;
					}
					phylos[num].state = THINKING;
					phylos[num].turn = 0;
					phylos[num].sem = sem_open(strandnum("sem_", num), 0);
					if (phylos[num].sem == NULL)
					{
						puts("Failed to open semaphore\n");
						break;
					}
					phylosopher(num);
					return 0;
				}
				else
				{
					children[num] = pid;
					*phylo_count = *phylo_count + 1;
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
			puts("removing\n");
			*phylo_count = *phylo_count - 1;
			sem_close(phylos[*phylo_count].sem);
			kill(children[*phylo_count], SIGKILL);
			waitpid(children[*phylo_count], NULL, 0);
			if (*phylo_count <= 0)
			{
				puts("No more phylos\n");
				leave();
			}

			break;
		}
		case 'q':
		{
			puts("quiting\n");
			leave();
		}
		default:
			break;
		}

		sem_post(mutex);
	}
	close(pipeFd[WRITE_END]);
	leave();
	exit(0);
}

void leave()
{
	for (unsigned int i = 0; i < *phylo_count; i++)
	{
		sem_close(phylos[i].sem);
		kill(children[i], SIGKILL);
		waitpid(children[i], NULL, 0);
	}
	kill(children[MAX_PHYLOS], SIGKILL);
	waitpid(children[MAX_PHYLOS], NULL, 0);
	free(phylos);
	sem_close(mutex);
	free(phylo_count);
	puts("byee!\n");
	exit(0);
}
