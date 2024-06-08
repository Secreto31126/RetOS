/* main.c */
#include <stdlib.h>
#include <string.h>
#include <sys.h>
#include "tests.h"
#include "test_util.h"

extern char bss;
extern char endOfBinary;
#define MIN_WAIT 1000000

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
		// 1000000 sounds about right idk the only way this function is used is via a single execv with argc 0
		uint64_t aux = 0;
		if (argc <= 1 || (aux = satoi(argv[1]) <= MIN_WAIT))
			endless_loop_print(1000000);
		endless_loop_print(aux);

		return 0;
	}
	if (!strcmp(test, "endless_loop"))
	{
		endless_loop();
		return 0;
	}

	if (!strcmp(test, "testmm"))
	{
		int aux = test_mm(argc - 1, argv + 1);
		puts("Test done.");
		if (aux < 0)
			puts("\nTest reported an error.");
		return aux;
	}
	if (!strcmp(test, "testsync"))
	{
		int aux = test_sync(argc - 1, argv + 1);
		puts("Test done.");
		if (aux < 0)
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
		if (aux < 0)
			puts("\nTest reported an error.");
		return aux;
	}

	puts("Unknown test");
	return 1;
}