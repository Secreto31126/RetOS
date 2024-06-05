/* main.c */
#include <stdio.h>
#include <stdlib.h>

extern char bss;
extern char endOfBinary;

#define puts(str) write(1, (str), strlen(str))

int main(int argc, char *argv[])
{
	puts("Tests module");
	return 0;
}