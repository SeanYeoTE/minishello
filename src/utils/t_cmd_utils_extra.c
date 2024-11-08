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

#include "../core/minishell.h"

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

int	create_cmd(t_shell *store, t_node *start, t_node *end, bool create)
{
	t_cmd	*new;

	if (!start)
		return (1);
	new = init_cmd(store, start, end, create);
	if (!new)
		return (1);
	return (0);
}

void	detach_redir(t_cmd *new)
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
			temp = new->command;
		}
		else
			temp = temp->next;
	}
}
