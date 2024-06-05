/* main.c */
#include <stdlib.h>
#include <string.h>
#include <sys.h>
#include "tests.h"
#include "test_util.h"

extern char bss;
extern char endOfBinary;

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
		endless_loop_print(1000000);
		return 0;
	}
	if (!strcmp(test, "endless_loop"))
	{
		endless_loop();
		return 0;
	}

	if (!strcmp(test, "testmm"))
	{
		// these args should probably be hardcoded, I haven't fully read the tests yet
		int aux = test_mm(argc - 1, argv + 1);
		puts("Test done.");
		if (aux < 0)
			puts("\nTest reported an error.");
		return aux;
	}
	if (!strcmp(test, "testsync"))
	{
		// these args should probably be hardcoded, I haven't fully read the tests yet
		int aux = test_sync(argc - 1, argv + 1);
		puts("Test done.");
		if (aux < 0)
			puts("\nTest reported an error.");
		return aux;
	}
	if (!strcmp(test, "testprio"))
	{
		test_prio();
		puts("Test done.");
		return 0;
	}
	if (!strcmp(test, "testprocesses"))
	{
		// these args should probably be hardcoded, I haven't fully read the tests yet
		int aux = test_processes(argc - 1, argv + 1);
		puts("Test done.");
		if (aux < 0)
			puts("\nTest reported an error.");
		return aux;
	}

	puts("Unknown test");
	return 1;
}