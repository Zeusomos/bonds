#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void p(void)
{
	char buffer[64];
	void *ptr;
	
	fflush(stdout);
	gets(buffer);
	ptr = __builtin_return_address(0);
	if (((unsigned int)ptr & 0xb0000000) == 0xb0000000)
	{
		printf("(%p)\n", &ptr);
		exit(1);
	}
	puts(buffer);
	strdup(buffer);
}

int main(void)
{
	p();
}
