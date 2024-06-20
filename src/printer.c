
#include "minishell.h"

int print_stack(t_node **head)
{
	t_node	*start;
	int		count;

	count = 0;
	start = *head;
	while (start)
	{
		// ft_printf("Node Counter: %d\n", count);
		printf("Node Value: %s\n", start->data);
		printf("Token Type: %d\n", start->type);
		printf("***************\n");
		start = start->next;
		count++;
	}
	printf("Total Nodes: %d\n", count);
	return (0);
}

int print_stack_se(t_node *start, t_node *end)
{
	int		count;

	count = 0;
	while (start != end)
	{
		// ft_printf("Node Counter: %d\n", count);
		printf("Node Value: %s\n", start->data);
		printf("Token Type: %d\n", start->type);
		printf("***************\n");
		start = start->next;
		count++;
	}
	printf("Total Nodes: %d\n", count);
	return (0);
}

int	print_argv(char **argv)
{
	while (*argv)
	{
		printf("%s\n", *argv);
		argv++;
	}
}