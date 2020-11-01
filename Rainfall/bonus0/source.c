#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char	*p(char *buf, char *string)
{
	char	read_buf[4096];

	puts(string);
	read(0, read_buf, 4096);
	*strchr(read_buf, '\n') = '\0';
	strncpy(buf, read_buf, 20);
}

char	*pp(char *buffer)
{
	char	*first;
	char	*second;
	int		len;

	p(first, " - ");
	p(second, " - ");
	strcpy(buffer, first);
	len = strlen(buffer);
	buffer[len] = ' ';
	buffer[len + 1] = '\0';
	strcat(buffer, second);
}

int	main(void)
{
	char buf[42];

	pp(buf);
	puts(buf);
	return (0);
}