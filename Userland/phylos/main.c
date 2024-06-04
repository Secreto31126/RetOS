/* main.c */

#include <sys.h>
#include <string.h>
#include <stdlib.h>
#include <semaphores.h>

extern char bss;
extern char endOfBinary;

#define puts(str) write(1, (str), strlen(str) - 1)

int main(int argc, char *argv[])
{
	// write(STD_OUT, "I'm a philosopher\n", sizeof("I'm a philosopher\n") - 1);
	// sem_t *sem = sem_open("phylos", 0);
	// int forked = fork();
	// if (forked == 0)
	// {
	// 	write(STD_OUT, "I'm suposed to be blocking now\n", sizeof("I'm suposed to be blocking now\n") - 1);
	// 	sem_wait(sem);
	// 	puts("I am a philosopher\n");
	// 	sem_post(sem);
	// 	sem_close(sem);
	// 	exit(0);
	// }
	// else
	// {
	// 	sleep(1);
	// 	write(STD_OUT, "You should unblock now\n", sizeof("You should unblock now\n") - 1);
	// 	sem_post(sem);
	// 	waitpid(forked, NULL, 0);
	// }
	puts("I'm a philosopher\n");
	exit(0);
}
