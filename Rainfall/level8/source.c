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
	