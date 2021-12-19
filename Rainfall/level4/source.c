
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int m;

int	p(char *buf)
{
	printf(buf);
}

int n(void)
{
	char buffer[512];
	
	fgets(buffer, 512, stdin);
	p(buffer);
	if (m == 16930116)
	{
		system("/bin/cat /home/user/level5/.pass");
	}
}

int main(void)
{
	n();
}