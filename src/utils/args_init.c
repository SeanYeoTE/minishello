/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:48:58 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/08 15:02:10 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

static void	setup_new_node(t_node *new_node, char *value)
{
	new_node->next = NULL;
	new_node->data = value;
	new_node->parent = NULL;
	new_node->prev = NULL;
}

int	init_node(char *value, t_node **head)
{
	t_node	*new_node;
	t_node	*prev_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return (-1);
	setup_new_node(new_node, value);
	if (*head == NULL)
		*head = new_node;
	else
	{
		prev_node = get_last(*head);
		prev_node->next = new_node;
		new_node->prev = prev_node;
	}
	return (0);
}

t_node	*get_last(t_node *last)
{
	t_node	*current;

	current = last;
	while (current && current->next)
		current = current->next;
	return (current);
}

t_node	*get_node(t_node *ret, int num)
{
	t_node	*current;
	int		count;

	current = ret;
	count = 0;
	while (current && count < num)
	{
		current = current->next;
		count++;
	}
	return (current);
}
