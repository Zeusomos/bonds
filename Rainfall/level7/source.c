#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

char			c[69];

void 			m(void)
{
	printf("%s - %d\n", c, (int)time(0));
}

int				main(int argc, char **argv)
{
	int		*first;
	int		*second;

	first = malloc(8);
	first[0] = 1;
	first[1] = *((int *)malloc(8));

	second = malloc(8);
	second[0] = 2;
	second[1] = *((int *)malloc(8));

	strcpy((char *)first + 1, argv[1]);
	strcpy((char *)second + 1, argv[2]);

	fgets(c, 68, fopen("/home/user/level8/.pass", "r"));
	puts("~~");
	return 0;
}