/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_wrapper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:02:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 21:31:00 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 

/**
 * @brief Updates shell exit status based on process status
 * @param store Shell data structure
 * @param status Process status from waitpid
 * @return int Exit status
 */
static int	return_exit_status(t_shell *store, int pid)
{
	int	status;

	waitpid(pid, &status, 0);
	signal(SIGINT, ctrl_c_handler);
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
 * @brief Creates a child process to handle heredoc input
 *
 * @param cmd Command structure containing heredoc information
 * @param store Shell data structure
 * @param child2 Flag indicating if this is a nested child process
 * @return int 0 on success, 1 on error
 */
int	heredoc_child(t_cmd *cmd, t_shell *store)
{
	pid_t	pid;
	int		pipe_fds[2];

	if (cmd->heredoc_fd > 0)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (pipe(pipe_fds) == -1)
		return (perror("pipe"), 1);
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
	return (return_exit_status(store, pid));
}

int	heredoc_child_loop(t_shell *store)
{
	pid_t	pid;
	t_cmd	*cmd;

	if (setup_heredoc_pipes_wrapper(store) != 0)
		return (1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		heredoc_finisher_wrapper(store);
	}
	cmd = store->cmd_head;
	while (cmd)
	{
		close_heredoc_write(cmd);
		cmd = cmd->next;
	}
	return (return_exit_status(store, pid));
}
