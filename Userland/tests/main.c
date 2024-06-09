/* main.c */
#include <stdlib.h>
#include <string.h>
#include <sys.h>
#include "tests.h"
#include "test_util.h"
#include "syscall.h"

extern char bss;
extern char endOfBinary;
#define MIN_WAIT 10

#define puts(str) write(1, (str), strlen(str))

int main(int argc, char *argv[])
{
	if (argc < 1)
	{
		puts("No test specified");
		return 1;
	}
	char *test = argv[0];

	// Remove trailing whitespace
	int truncate = 0;
	while (test[truncate] && test[truncate] != ' ')
		truncate++;
	test[truncate] = 0;

	if (!strcmp(test, "my_process_inc"))
	{
		return my_process_inc(argc - 1, argv + 1);
	}
	if (!strcmp(test, "endless_loop_print"))
	{
		my_nice(my_getpid(), 19); // Since I'm not too eager to give extra cpu time to a process stuck in a busy wait
		uint64_t aux = 0;
		if (argc <= 1 || (aux = satoi(argv[1])) <= MIN_WAIT)
			endless_loop_print(MIN_WAIT);
		endless_loop_print(aux);

		return 0;
	}
	if (!strcmp(test, "endless_loop"))
	{
		my_nice(my_getpid(), 19); // Since I'm not too eager to give extra cpu time to a process stuck in a busy wait
		endless_loop();
		return 0;
	}

	if (!strcmp(test, "testmm"))
	{
		int aux = test_mm(argc - 1, argv + 1);
		puts("Test done.");
		if (aux == -2)
			puts("\nTest reported incorrect parameters. This test expects one parameter: max_memory");
		else if (aux < 0)
			puts("\nTest reported an error.");
		return aux;
	}
	if (!strcmp(test, "testsync"))
	{
		int aux = test_sync(argc - 1, argv + 1);
		puts("Test done.");
		if (aux == -2)
			puts("\nTest reported incorrect parameters. This test expects two parameters: inc, use_sem");
		else if (aux < 0)
			puts("\nTest reported an error.");
		return aux;
	}
	if (!strcmp(test, "testprio"))
	{
		test_prio(argc - 1, argv + 1);
		puts("Test done.");
		return 0;
	}
	if (!strcmp(test, "testprocesses"))
	{
		int aux = test_processes(argc - 1, argv + 1);
		puts("Test done.");
		if (aux == -2)
			puts("\nTest reported incorrect parameters. This test expects one parameter: max_processes");
		else if (aux < 0)
			puts("\nTest reported an error.");
		return aux;
	}

	puts("Unknown test");
	return 1;
}