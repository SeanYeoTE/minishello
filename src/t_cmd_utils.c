/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 18:40:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/07/21 16:29:52 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*get_last_cmd(t_cmd *cmd)
{
	while (cmd)
		cmd = cmd->next;
	return (cmd);
}

t_cmd	*init_cmd(t_shell *store, t_node *start, t_node *end)
{
	t_cmd	*cmd;
	// puts("init_cmd\n");
	t_cmd	*last;
	last = get_last_cmd(store->cmd_head);
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (last == NULL)
	{
		puts("1");
		// initialising for single command
		cmd->prev = NULL;
		cmd->next = NULL;
		store->cmd_head = cmd;
	}
	else
	{
		puts("2");
		// second executable commands onwards
		cmd->prev = last;
		cmd->next = NULL;
	}

	// doesnt seem to be required
	// store->cmd_tail = cmd;

	cmd->command = start;
	if (end->next)
		end->next = NULL;
	// print_stack(&cmd->command);
	detach_redir(cmd);
	return (cmd);
}

int	create_cmd(t_shell *store, t_node *start, t_node *end)
{
	t_cmd	*new;
	t_node	*temp;
	
	if (start == NULL)
		return (1);
	else
	{
		init_cmd(store, start, end);
		return (0);
	}
}

void	detach_redir(t_cmd *new)
{
	t_node	*temp;
	
	temp = new->command;
	while (temp)
	{
		if (redir_checker(temp) == 1)
		{
			puts("detach_redir");
			new->redir = temp;
			temp->prev->next = new->redir->next->next;
			if (temp->next->next)
				temp->next->next->prev = temp->prev;
			
			new->redir->next->next = NULL;
			new->redir->prev = NULL;
		}
		temp = temp->next;
	}
}