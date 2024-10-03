/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 18:40:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/03 14:05:04 by seayeo           ###   ########.fr       */
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
	t_cmd	*last;
	
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
	{
		fprintf(stderr, "ERROR: Failed to allocate memory for cmd\n");
		return NULL;
	}
	cmd->redir = NULL;
	if (create)
	{
		cmd->prev = NULL;
		cmd->next = NULL;
		store->cmd_head = cmd;
	}
	else
	{
		last = get_last_cmd(store->cmd_head);
		cmd->prev = last;
		cmd->next = NULL;
		last->next = cmd;
	}
	cmd->command = start;
	
	if (end->next)
		end->next = NULL;
	detach_redir(cmd);
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->heredoc_fd = -1;
	return (cmd);
}

int	create_cmd(t_shell *store, t_node *start, t_node *end, bool create)
{
	t_cmd	*new;
	
	if (start == NULL)
	{
		return (1);
	}
	else
	{
		new = init_cmd(store, start, end, create);
		if (!new)
		{
			return (1);
		}
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
			new->redir = temp;
			if (temp->prev)
			{
				temp->prev->next = NULL;
			}
			break;
		}
		temp = temp->next;
	}
}
