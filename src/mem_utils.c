/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:48:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/09/15 14:54:27 by seayeo           ###   ########.fr       */
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

void	free_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;
	t_cmd	*current;

	if (NULL == cmd)
		return ;
	current = *cmd;
	while (current)
	{
		tmp = current->next;
		free(current);
		current = tmp;
	}
	*cmd = NULL;
}

void	free_nonessential(t_shell *store)
{
	freechararray(store->paths);		
	if (store->head != NULL)
		free_stack(&(store->head));
	// if (store->cmd_head != NULL)
	// 	free_cmd(&(store->cmd_head));
	// free(store);
}