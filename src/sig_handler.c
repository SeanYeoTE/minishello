#include "minishell.h"

void	ctrl_c_handler(int signum)
{
	if (signum == SIGINT)
		write (STDERR_FILENO, "\n", 1);
	// printf ("HI\n");
	return ;
}