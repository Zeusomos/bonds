#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int				main(int argc, char **argv)
{
	FILE *file;
	char buffer[132];
	int	 arg;

	file = fopen("/home/user/end/.