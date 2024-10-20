/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 18:40:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/20 17:43:34 by seayeo           ###   ########.fr       */
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

int is_file(const char *path)
{
	struct stat path_stat;

	// Attempt to get the file status
	if (stat(path, &path_stat) != 0)
	{
		// Return 1 even if the file does not exist
		return 1; // Path doesn't exist, but we're treating it as a valid case
	}

	// Check if the path is a regular file
	if (S_ISREG(path_stat.st_mode))
	{
		return 1; // It is a regular file
	}
	else
	{
		return 0; // It is not a regular file
	}
}


void	set_cmdlinked(t_node *start, t_node *temp)
{
	t_node	*curr_last;
	
	temp->prev->next = NULL;
	curr_last = get_last(start);
	
	if (temp->next)
	{
		curr_last->next = temp->next->next;	
	}
	else
		curr_last->next = temp->next;
	
	if (curr_last->next)
		curr_last->next->prev = curr_last;
}

void	set_redirlinked(t_node *redir, t_node *temp)
{
	t_node	*file;
	t_node	*last;
	
	if (!redir)
	{
		redir = temp;
	}
	else
	{
		last = get_last(redir);
		last->next = temp;
	}
	file = temp->next;
	if (file)
	{
		file->next = NULL; 
	}
		
}

void detach_redir(t_cmd *new)
{
	t_node	*temp;
	t_node	*start;
	t_node	*last_redir;
	t_node	*file;

	temp = new->command;
	start = temp;

	while (temp)
	{
		if (redir_checker(temp) == 1)
		{
			set_cmdlinked(start, temp);
			
			set_redirlinked(new->redir, temp);
			temp = start; // Reset temp to start for the next iteration
		}
		else
		{
			temp = temp->next; // Move to the next node
		}
	}
}