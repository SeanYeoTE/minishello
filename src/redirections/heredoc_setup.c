/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_setup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 10:00:00 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 16:16:37 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Sets up pipes for a command's heredoc
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
 * @brief Creates a new pipe for heredoc operations
 * @param pipe_fds Array to store pipe file descriptors
 * @return int 0 on success, 1 on error
 */
int	setup_heredoc_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
		return (perror("pipe"), 1);
	return (0);
}

/**
 * @brief Closes heredoc file descriptor if open
 * @param cmd Command structure containing the file descriptor
 */
void	cleanup_heredoc_fd(t_cmd *cmd)
{
	if (cmd->heredoc_fd > 0)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
}

/**
 * @brief Closes all write file descriptors in the command list
 * @param store Shell data structure containing command list
 */
void	cleanup_write_fds(t_shell *store)
{
	t_cmd	*cmd;

	cmd = store->cmd_head;
	while (cmd)
	{
		if (cmd->heredoc_write_fd > 0)
		{
			close(cmd->heredoc_write_fd);
			cmd->heredoc_write_fd = -1;
		}
		cmd = cmd->next;
	}
}

/**
 * @brief Sets up heredoc pipes for all commands in the list
 * @param store Shell data structure containing command list
 * @return int 0 on success, 1 on error
 */
int	setup_all_heredoc_pipes(t_shell *store)
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
