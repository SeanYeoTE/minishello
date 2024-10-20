#include "minishell.h"

int	echo_handler(t_node *current, t_node *end)
{
	bool	n_flag;
	bool	printed;

	n_flag = false;
	current = current->next;
	if (current)
	{
		if (ft_strncmp(current->data, "-n", 2) == 0)
			n_flag = true;
	}
	if (n_flag)
		current = current->next;
	while (current != NULL)
	{
		ft_putstr_fd(current->data, 1);
		if (current->next)
			ft_putchar_fd(' ', 1);
		current = current->next;  
	}
	printed = false;
	if (!printed && !n_flag)
		ft_putchar_fd('\n', 1);
	return (EXIT_SUCCESS);
}