
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void n(void)
{
	system("/bin/cat /home/user/level7/.pass");
}

void m(void)
{
	puts("Nope");
}

int		main(int argc, char **argv)
{
	void (*function)(void*);
	void *buffer;

	buffer = malloc(64);
	function = malloc(4);
	function = m;
	strcpy(buffer, argv[1]);
	function;
}