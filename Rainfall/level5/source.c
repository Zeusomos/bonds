#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int o(void)
{
	system("/bin/sh");
	_exit(1);
}

int n(void)
{
	char buffer[512];
	
	fgets(buffer, 512, stdin);
	printf(buffer);
	exit(1);
}

int main(void)
{
	n();
}