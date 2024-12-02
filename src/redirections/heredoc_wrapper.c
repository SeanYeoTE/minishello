/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_wrapper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:02:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 18:33:16 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 



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
	int		status;

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
	{
		close(pipe_fds[0]);
		cmd->heredoc_write_fd = pipe_fds[1];
		exit_wrapper(store, heredoc_finisher(cmd, store));
	}
	close(pipe_fds[1]);
	cmd->heredoc_fd = pipe_fds[0];
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		store->exit_status = WEXITSTATUS(status);
		return (WEXITSTATUS(status));
	}
	signal(SIGINT, ctrl_c_handler);
	store->exit_status = 1;
	return (1);
}

int	heredoc_child_loop(t_shell *store)
{
	pid_t	pid;
	t_cmd	*cmd;
	int		result;
	int		status;

	cmd = store->cmd_head;
	while (cmd)
	{
		if (setup_heredoc_pipes(cmd) != 0)
			return (1);
		cmd = cmd->next;
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
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
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		store->exit_status = WEXITSTATUS(status);
		return (WEXITSTATUS(status));
	}
	signal(SIGINT, ctrl_c_handler);
	store->exit_status = 10000;
	return (1);
}
