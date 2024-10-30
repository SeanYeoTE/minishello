/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 18:40:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/29 22:27:48 by seayeo           ###   ########.fr       */
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

void	set_parent(t_node *node, t_cmd *cmd)
{
	while (node)
	{
		node->parent = cmd;
		node = node->next;
	}
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
	cmd->input_changed = false;
	set_parent(cmd->command, cmd);
	if (end->next)
		end->next = NULL;
	detach_redir(cmd);
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->heredoc_fd = -1;
	cmd->heredoc_delimiter = NULL;
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

void	remove_nodes(t_node **start, t_node *redir, t_node *file)
{
	if (redir->prev)
	{
		if (file)
			redir->prev->next = file->next;
		else
			redir->prev->next = NULL;
	}
	else
	{
		if (file)
			*start = file->next;
		else
			*start = NULL;
	}

	if (file && file->next)
		file->next->prev = redir->prev;

	redir->prev = NULL;
	if (file)
		file->next = NULL;
}

void	add_to_redir(t_node **redir, t_node *new_redir, t_node *new_file)
{
	t_node *last;

	if (!*redir)
	{
		*redir = new_redir;
		if (new_file)
			new_redir->next = new_file;
	}
	else
	{
		last = *redir;
		while (last->next)
			last = last->next;
		last->next = new_redir;
		if (new_file)
		{
			new_redir->next = new_file;
			new_file->next = NULL;
		}
		else
			new_redir->next = NULL;
	}
}

void detach_redir(t_cmd *new)
{
	t_node	*temp;
	t_node	*file;

	temp = new->command;
	while (temp)
	{
		if (redir_checker(temp) == 1)
		{
			file = temp->next;
			
			remove_nodes(&new->command, temp, file);
			add_to_redir(&new->redir, temp, file);
			// print_stack(&new->redir);
			temp = new->command;
		}
		else
		{
			temp = temp->next;
		}
	}
}
