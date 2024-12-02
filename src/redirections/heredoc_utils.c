/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 18:32:17 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 21:26:20 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Closes the heredoc write file descriptor if it's open
 * 
 * @param cmd Command structure containing the file descriptor
 */
void	close_heredoc_write(t_cmd *cmd)
{
	if (cmd->heredoc_write_fd > 0)
	{
		close(cmd->heredoc_write_fd);
		cmd->heredoc_write_fd = -1;
	}
}

/**
 * @brief Sets up pipes for a command's heredoc
 * 
 * @param cmd Command structure to set up pipes for
 * @return int 0 on success, 1 on error
 */
int	setup_heredoc_pipes(t_cmd *cmd)
{
	int		pipe_fds[2];

	if (checkforheredoc(cmd) == 0)
		return (0);
	if (cmd->heredoc_fd > 0)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (cmd->heredoc_write_fd > 0)
	{
		close(cmd->heredoc_write_fd);
		cmd->heredoc_write_fd = -1;
	}
	if (pipe(pipe_fds) == -1)
		return (perror("pipe"), 1);
	cmd->heredoc_write_fd = pipe_fds[1];
	cmd->heredoc_fd = pipe_fds[0];
	return (0);
}

/**
 * @brief Check if this is the last heredoc in the command
 *
 * @param current Current node in redirection list
 * @return int 1 if last heredoc, 0 otherwise
 */
int	is_last_heredoc(t_node *current)
{
	t_node	*tmp;

	tmp = current->next->next;
	while (tmp)
	{
		if (ft_strcmp(tmp->data, "<<") == 0)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

int	setup_heredoc_pipes_wrapper(t_shell *store)
{
	t_cmd	*cmd;

	cmd = store->cmd_head;
	while (cmd)
	{
		if (setup_heredoc_pipes(cmd) != 0)
			return (1);
		cmd = cmd->next;
	}
	return (0);
}

void	heredoc_finisher_wrapper(t_shell *store)
{
	t_cmd	*cmd;
	int		result;

	cmd = store->cmd_head;
	while (cmd)
	{
		close(cmd->heredoc_fd);
		if (checkforheredoc(cmd))
		{
			result = heredoc_finisher(cmd, store);
			if (result != EXIT_SUCCESS)
				exit_wrapper(store, result);
		}
		cmd = cmd->next;
	}
	exit_wrapper(store, EXIT_SUCCESS);
}
