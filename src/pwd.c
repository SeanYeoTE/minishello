#include "minishell.h"

int	pwd_handler(t_node *current)
{
	char	*cwd;
	size_t	cwd_buf;

	cwd_buf = 1000;
	cwd = malloc (cwd_buf * sizeof(char));
	if (cwd == NULL)
	{
		perror("Invalid Memory Allocation\n");
		return (1);
	}
	if (getcwd(cwd, cwd_buf) != NULL)
		printf("%s\n", cwd);
	else
	{
		perror("PWD Error\n");
		free(cwd);
		return (1);
	}
	return (0);
}