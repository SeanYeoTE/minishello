/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:05:29 by seayeo            #+#    #+#             */
/*   Updated: 2024/09/20 13:42:24 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_counter(t_node *loop)
{
	int	count;
	
	count = 0;
	while (loop)
	{
		if (ft_strcmp(loop->data, "|") == 0)
			count++;
		loop = loop->next;
	}
	return (count);
}

int	wait_for_command(pid_t pid)
{
	int	status;
	
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		t_exit_status = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		t_exit_status = WTERMSIG(status) + 128;
	}
	return (EXIT_SUCCESS);
}

void	run_cmd(t_cmd *cmd, t_shell *store)
{
	if (check_builtin(cmd->command) == 0)
	{
		t_exit_status = executor(store, cmd->command, NULL);
		exit(t_exit_status);
	}
	else
	{
		t_exit_status = builtin_main(store, cmd->command, cmd->redir);
		exit(t_exit_status);
	}
}

void	setup_pipes(int in_fd, int out_fd, t_cmd *cmd)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}

	if (cmd->redir && cmd->output_fd != STDOUT_FILENO)
	{
		dup2(cmd->output_fd, STDOUT_FILENO);
		close(cmd->output_fd);
	}
	else if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}

int	execute_command(t_shell *store, t_cmd *cmd, int in_fd, int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("Fork failed");
		return EXIT_FAILURE;
	}
	
	if (pid == 0)
	{
		setup_pipes(in_fd, out_fd, cmd);
		redir_handler(cmd, cmd->redir, NULL);
		run_cmd(cmd, store);
	}
	
	return pid;
}

int	multi_executor(t_shell *store, int num_pipes)
{
	int		pipe_fds[2];
	int		in_fd = STDIN_FILENO;
	t_cmd	*cmd = store->cmd_head;
	pid_t	last_pid;

	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(pipe_fds) == -1)
			{
				perror("Pipe failed");
				return EXIT_FAILURE;
			}
		}

		int out_fd = cmd->next ? pipe_fds[1] : STDOUT_FILENO;

		last_pid = execute_command(store, cmd, in_fd, out_fd);
		if (last_pid == EXIT_FAILURE)
			return EXIT_FAILURE;

		if (in_fd != STDIN_FILENO)
			close(in_fd);

		if (cmd->next)
		{
			close(pipe_fds[1]);
			in_fd = pipe_fds[0];
		}

		if (!cmd->next)  // Only wait for the last command
			wait_for_command(last_pid);
		else
			waitpid(last_pid, NULL, 0);  // Wait but don't update exit status

		cmd = cmd->next;
	}

	return EXIT_SUCCESS;
}