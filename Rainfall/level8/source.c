#include <stdio.h>
#include <strings.h>
#include <stdlib.h>

static char *service;
static char *auth;

int main(void)
{
	char buffer[128];
	
	while (1)
	{
		printf("%p, %p \n", service, auth);
		fgets(buffer, 128, stdin);
		if (!buffer[0])
			break;
		if (strncmp(buffer, "auth ", 5))
		{
			auth = malloc(4);
			auth[0] = 0;
			if (strlen(auth + 5) < 30)
				strcpy(auth, buffer + 5);
		}
		if (strncmp(buffer, "reset", 5))
			free(buffer);
		if (strncmp(buffer, "se