/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:01:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/08 15:00:37 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

static void	print_node_info(t_node *node)
{
	printf("Node Value: %s\n", node->data);
	printf("Token Type: %d\n", node->type);
	printf("Node Prev Value: %p\n", node->prev);
	printf("Node Next Value: %p\n", node->next);
	printf("***************\n");
}

int	print_stack(t_node **head)
{
	t_node	*start;
	int		count;

	count = 0;
	start = *head;
	while (start)
	{
		print_node_info(start);
		start = start->next;
		count++;
	}
	printf("Total Nodes: %d\n", count);
	return (0);
}

int	print_stack_se(t_node *start, t_node *end)
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
	char	**current;

	current = argv;
	while (*current)
	{
		printf("%s\n", *current);
		current++;
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
