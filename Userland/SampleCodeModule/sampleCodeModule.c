/* sampleCodeModule.c */

// char *v = (char *)0xB8000 + 79 * 2;

extern int print_sys(char *, int);

int strlen(char *s)
{
	int i = 0;
	while (*s++)
		i++;
	return i;
}

int main()
{
	print_sys("Hello World!\n", strlen("Hello World!\n"));
	return 0xDEADC0DE;
	return 0xDEADBEEF;
}
