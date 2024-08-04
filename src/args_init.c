/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:48:58 by seayeo            #+#    #+#             */
/*   Updated: 2024/08/04 16:45:31 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_node(char *value, t_node **head)
{
	t_node	*new_node;
	t_node	*prev_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)

		return (-1);
	new_node->next = NULL;
	new_node->data = value;
	if (*head == NULL)
	{
		new_node->prev = NULL;
		*head = new_node;
	}
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
	while (last->next != NULL)
		last = last->next;
	return (last);
}

t_node	*get_node(t_node *ret, int num)
{
	int	x;
	
	x = 0;
	while (ret->next != NULL)
	{
		if (x == num)
			break ;
		ret = ret->next;
		x++;
	}
	return (ret);
}

t_node	*remove_node(t_shell *store, t_node *node)
{
	t_node	*prev;
	t_node	*next;

	prev = node->prev;
	next = node->next;
	if (prev)
		prev->next = next;
	if (next)
		next->prev = prev;
	if (node == store->head)
		store->head = next;
	if (node == store->tail)
		store->tail = prev;
	free(node);
	return (next);
}