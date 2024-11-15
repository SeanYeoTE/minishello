/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 13:48:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/08 14:57:53 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

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

	if (!stack)
		return ;
	current = *stack;
	while (current)
	{
		tmp = current->next;
		free(current->data);
		free(current);
		current = tmp;
	}
	*stack = NULL;
}

void	free_cmd(t_cmd **cmd)
{
	t_cmd	*tmp;
	t_cmd	*current;

	if (!cmd)
		return ;
	current = *cmd;
	while (current)
	{
		tmp = current->next;
		free_stack(&current->command);
		if (current->redir)
			free_stack(&current->redir);
		if (current->heredoc_delimiter)
			free(current->heredoc_delimiter);
		free(current);
		current = tmp;
	}
	*cmd = NULL;
}
