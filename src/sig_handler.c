#include "minishell.h"

void	ctrl_c_handler(int signum)
{
	printf("\n");
	if (signal(SIGINT, ctrl_c_handler) == SIG_ERR)
	{
		perror ("Error re-registering SIGINT"\n);
		exit(FAILURE);
	}
	
}

void	register_sigint(void)
{
	if (signal(SIGINT, ctrl_c_handler) == SIG_ERR)
	{
		perror("Error registering SIGINT\n");
		exit (FAILURE);
	}
}