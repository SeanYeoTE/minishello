/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 18:40:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/07/24 16:15:25 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*get_last_cmd(t_cmd *cmd)
{
	while (cmd->next)
		cmd = cmd->next;
	return (cmd);
}

int	count_cmds(t_shell *store)
{
	t_cmd	*iter;
	int		count;
	
	count = 0;
	iter = store->cmd_head;
	while (iter)
	{
		count++;
		iter = iter->next;
	}
	return (count);
}

t_cmd	*init_cmd(t_shell *store, t_node *start, t_node *end, bool create)
{
	t_cmd	*cmd;
	// puts("init_cmd\n");
	t_cmd	*last;
	
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (create)
	{
		// puts("1");
		// initialising for single command
		cmd->prev = NULL;
		cmd->next = NULL;
		store->cmd_head = cmd;
	}
	else
	{
		// puts("2");
		last = get_last_cmd(store->cmd_head);
		// second executable commands onwards
		cmd->prev = last;
		cmd->next = NULL;
		last->next = cmd;
	}

	// doesnt seem to be required
	// store->cmd_tail = cmd;

	cmd->command = start;
	if (end->prev)
		end->prev->next = NULL;
	// print_stack(&cmd->command);
	detach_redir(cmd);
	return (cmd);
}

int	create_cmd(t_shell *store, t_node *start, t_node *end, bool create)
{
	t_cmd	*new;
	t_node	*temp;
	
	if (start == NULL)
		return (1);
	else
	{
		init_cmd(store, start, end, create);
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
