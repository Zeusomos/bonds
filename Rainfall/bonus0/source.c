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
	strncpy(buf, read_buf