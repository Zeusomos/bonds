
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int	main(int argc, char **argv)
{
	char	*exec_args[2];
	gid_t	egid;
	uid_t	euid;

	if (atoi(argv[1]) == 423)
	{
		exec_args[0] = strdup("/bin/sh");
		exec_args[1] = NULL;
		egid = getegid();
		euid = geteuid();
		setresuid(euid, euid, euid);
		setresgid(egid, egid, egid);
		execv("/bin/sh", exec_args);
	}
	else
	{
		fwrite("No !\n", 1, 5, stderr);
	}
	return (0);
}