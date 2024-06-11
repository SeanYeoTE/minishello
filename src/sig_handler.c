#include "minishell.h"

void	ctrl_c_handler(int signum)
{
	if (signum == SIGINT)
		write (STDERR_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}