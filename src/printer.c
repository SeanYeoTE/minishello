#include "minishell.h"

int print_stack(t_node **head)
{
	t_node	*start;
	int		count;

	count = 0;
	start = *head;
	while (start)
	{
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
		printf("Node Value: %s...\n", start->data);
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
	return (0);
}

int	print_cmd_stack(t_cmd **head)
{
	t_cmd	*start;
	int		count;

	count = 0;
	start = *head;
	while (start)
	{
		printf("Command: %d\n", count);
		print_stack(&start->command);
		if (start->redir)
		{
			printf("Redirection:\n");
			print_stack(&start->redir);
		}
		printf("***************\n");
		start = start->next;
		count++;
	}
	printf("Total Commands: %d\n", count);
	return (0);
}

int	print_error(char *str, char *arg)
{
	if (arg)
		printf("%s: %s\n", arg, str);
	else
		printf("minishell: %s\n", str);
	return (EXIT_FAILURE);  // Changed back to EXIT_FAILURE
}