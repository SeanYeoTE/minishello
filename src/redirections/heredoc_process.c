/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 10:00:00 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 16:33:45 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Updates shell exit status based on process status
 * @param store Shell data structure
 * @param status Process status from waitpid
 * @return int Exit status
 */
static int	return_exit_status(t_shell *store, int status)
{
	if (WIFEXITED(status))
	{
		store->exit_status = WEXITSTATUS(status);
		return (WEXITSTATUS(status));
	}
	store->exit_status = 1;
	return (1);
}

/**
 * @brief Handles child process operations for heredoc
 * @param cmd Command structure
 * @param store Shell data structure
 * @param pipe_fds Pipe file descriptors
 * @return int Result of heredoc operation
 */
int	handle_child_heredoc(t_cmd *cmd, t_shell *store, int pipe_fds[2])
{
	close(pipe_fds[0]);
	cmd->heredoc_write_fd = pipe_fds[1];
	return (heredoc_finisher(cmd, store));
}

/**
 * @brief Processes heredocs for all commands in the list
 * @param store Shell data structure containing command list
 * @return int EXIT_SUCCESS on success, non-zero on error
 */
int	process_heredocs(t_shell *store)
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
				return (result);
		}
		cmd = cmd->next;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Creates child process for heredoc handling
 * @param cmd Command structure
 * @param store Shell data structure
 * @return int 0 on success, non-zero on error
 */
int	heredoc_child(t_cmd *cmd, t_shell *store)
{
	pid_t	pid;
	int		pipe_fds[2];
	int		status;

	cleanup_heredoc_fd(cmd);
	if (setup_heredoc_pipe(pipe_fds) != 0)
		return (1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return (perror("fork"), 1);
	}
	if (pid == 0)
		exit_wrapper(store, handle_child_heredoc(cmd, store, pipe_fds));
	close(pipe_fds[1]);
	cmd->heredoc_fd = pipe_fds[0];
	waitpid(pid, &status, 0);
	signal(SIGINT, ctrl_c_handler);
	return (return_exit_status(store, status));
}

/**
 * @brief Creates child process to handle heredocs for all commands
 * @param store Shell data structure
 * @return int 0 on success, non-zero on error
 */
int	heredoc_child_loop(t_shell *store)
{
	pid_t	pid;
	int		status;

	if (setup_all_heredoc_pipes(store))
		return (1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		exit_wrapper(store, process_heredocs(store));
	cleanup_write_fds(store);
	waitpid(pid, &status, 0);
	signal(SIGINT, ctrl_c_handler);
	return (return_exit_status(store, status));
}
