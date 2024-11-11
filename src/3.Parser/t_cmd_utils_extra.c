/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd_utils_extra.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:06:10 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/08 15:06:10 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 
void	set_parent(t_node *node, t_cmd *cmd)
{
	t_node	*current;

	current = node;
	while (current)
	{
		current->parent = cmd;
		current = current->next;
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
	t_node	*last;

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
