/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:05:29 by seayeo            #+#    #+#             */
/*   Updated: 2024/09/18 15:39:41 by seayeo           ###   ########.fr       */
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

int	wait_for_pipes(t_shell *store, int amount)
{
	int	i;
	int	status;
	
	i = 0;
	while (i < amount)
	{
		waitpid(store->pid[i], &status, 0);
		if (WIFEXITED(status))
		{
			t_exit_status = WEXITSTATUS(status);
		}
		else if (WIFSIGNALED(status))
		{
			t_exit_status = WTERMSIG(status) + 128;
		}
		i++;
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

int	execute_command(t_shell *store, t_cmd *cmd, int in_fd, int out_fd, int i)
{
	store->pid[i] = fork();
	if (store->pid[i] < 0)
	{
		perror("Fork failed");
		return EXIT_FAILURE;
	}
	
	if (store->pid[i] == 0)
	{
		setup_pipes(in_fd, out_fd, cmd);
		redir_handler(cmd, cmd->redir, NULL);
		run_cmd(cmd, store);
	}
	
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != STDOUT_FILENO)
		close(out_fd);
	
	return EXIT_SUCCESS;
}

int	multi_executor(t_shell *store, int num_pipes)
{
	int		pipe_fds[2][2];
	int		in_fd = STDIN_FILENO;
	int		i = 0;
	t_cmd	*cmd = store->cmd_head;

	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(pipe_fds[i % 2]) == -1)
			{
				perror("Pipe failed");
				return EXIT_FAILURE;
			}
		}

		int out_fd = cmd->next ? pipe_fds[i % 2][1] : STDOUT_FILENO;

		if (execute_command(store, cmd, in_fd, out_fd, i) == EXIT_FAILURE)
			return EXIT_FAILURE;

		if (cmd->next)
		{
			close(pipe_fds[i % 2][1]);
			in_fd = pipe_fds[i % 2][0];
		}

		cmd = cmd->next;
		i++;
	}

	return wait_for_pipes(store, i);
}