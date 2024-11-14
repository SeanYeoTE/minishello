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

/**
 * @brief Sets the parent command for a sequence of nodes
 * @param node Starting node in the sequence
 * @param cmd Command structure to set as parent
 * @details Iterates through linked list of nodes, setting each node's parent
 * pointer to the provided command structure
 */
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

/**
 * @brief Removes redirection and file nodes from command sequence
 * @param start Pointer to start of command sequence
 * @param redir Redirection node to remove
 * @param file File node associated with redirection to remove
 * @details Updates node links to maintain list integrity after removal
 * Handles cases where nodes are at start, middle, or end of sequence
 */
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

/**
 * @brief Adds redirection and file nodes to redirection list
 * @param redir Pointer to redirection list head
 * @param new_redir New redirection node to add
 * @param new_file New file node associated with redirection
 * @details If list is empty, sets new nodes as head
 * Otherwise appends nodes to end of existing list
 */
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

/**
 * @brief Finds the last command in a command list
 * @param cmd Starting command in the list
 * @return Pointer to last command in list
 * @details Traverses command list following next pointers until end is reached
 */
t_cmd	*get_last_cmd(t_cmd *cmd)
{
	t_cmd	*current;

	current = cmd;
	while (current && current->next)
		current = current->next;
	return (current);
}

/**
 * @brief Counts total number of commands in shell
 * @param store Main shell data structure
 * @return Number of commands in command list
 * @details Traverses command list from head to tail, counting each command
 */
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
