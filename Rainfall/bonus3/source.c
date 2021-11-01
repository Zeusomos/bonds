#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int				main(int argc, char **argv)
{
	FILE *file;
	char buffer[132];
	int	 arg;

	file = fopen("/home/user/end/.pass", "r");
	bzero(buffer, 132);
	if (file != 0 || argc != 2)
		return (-1);
	fread(buffer, 1, 66, file);
	buffer[65] = '\0';
	buffer[atoi(argv[1])] = 0;
	fread(&buffer[66], 1, 65, file);
	fclose(file);
	if (strcmp(buffer, argv[1]) == 0)
		execl("/bin/sh", "sh", NULL);
	else
		puts(&buffer[66]);
	return 0;
}