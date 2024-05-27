/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:48:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/23 16:48:55 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	freechararray(char **v)
{
	char	**tmp;

	if (!v)
		return ;
	tmp = v;
	while (*tmp)
	{
		free(*tmp);
		tmp++;
	}
	free(v);
}

void	free_stack(t_node **stack)
{
	t_node	*tmp;
	t_node	*current;

	if (NULL == stack)
		return ;
	current = *stack;
	while (current)
	{
		tmp = current->next;
		free(current);
		current = tmp;
	}
	*stack = NULL;
}

void	free_nonessential(t_shell *store)
{
	freechararray(store->paths);
	if (store->head != NULL)
		free_stack(&store->head);
}