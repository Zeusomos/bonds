#include <stdio.h>

int	run(void)
{
	fwrite("Good... Wait what?\n", 1, 19, stdout);
	system("/bin/sh");
}

int	main(void)
{
	char buffer[64];
	gets(buffer);
}
