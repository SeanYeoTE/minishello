/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 18:40:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 14:28:35 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 
t_cmd	*get_last_cmd(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current && current->next)
		current = current->next;
	return (current);
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

static void	init_cmd_fds(t_cmd *cmd)
{
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->heredoc_fd = -1;
	cmd->heredoc_delimiter = NULL;
}

static void	setup_cmd_links(t_shell *store, t_cmd *cmd, bool create)
{
	t_cmd	*last;

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
}

t_cmd	*init_cmd(t_shell *store, t_node *start, t_node *end, bool create)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
	{
		fprintf(stderr, "ERROR: Failed to allocate memory for cmd\n");
		return (NULL);
	}
	cmd->redir = NULL;
	setup_cmd_links(store, cmd, create);
	cmd->command = start;
	cmd->input_changed = false;
	set_parent(cmd->command, cmd);
	if (end->next)
		end->next = NULL;
	detach_redir(cmd);
	init_cmd_fds(cmd);
	return (cmd);
}
