#include "minishell.h"

int	echo_handler(t_node *current, t_node *end)
{
	int		option;
	bool	printed;

	option = 0;
	current = current->next;
	if (ft_strncmp(current->data, "-n", 2) == 0)
		option = 1;
	if (option == 1)
		current = current->next;
	while (current != end)
	{
		printed = true;
		printf("%s", current->data);
		if (current->next)
			printf(" ");
		current = current->next;  
	}
	if (option == 0 || printed == false)
		printf("\n");
	return (EXIT_SUCCESS);
}