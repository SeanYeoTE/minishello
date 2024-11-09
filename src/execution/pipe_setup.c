/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:26:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/09 20:15:13 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

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

int	setup_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
	{
		print_error("Pipe failed", strerror(errno));
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	handle_pipe_fds(int *in_fd, int pipe_fds[2], int is_last_cmd)
{
	if (*in_fd > 2)
		close(*in_fd);
	if (!is_last_cmd)
	{
		if (pipe_fds[1] > 2)
			close(pipe_fds[1]);
		*in_fd = pipe_fds[0];
	}
	else
	{
		// if (pipe_fds[0] > 2)
		// 	close(pipe_fds[0]);
		// if (pipe_fds[1] > 2)
		// 	close(pipe_fds[1]);
	}
}

int	wait_for_command(pid_t pid)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = WTERMSIG(status) + 128;
	signal(SIGINT, ctrl_c_handler);
	return (EXIT_SUCCESS);
}

int	handle_command(t_shell *store, t_cmd *cmd, int *in_fd, int *out_fd)
{
	int		pipe_fds[2];
	int		is_last_cmd;

	is_last_cmd = (cmd->next == NULL);
	if (!is_last_cmd)
	{
		if (setup_pipe(pipe_fds) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		*out_fd = pipe_fds[1];
	}
	else
		*out_fd = STDOUT_FILENO;
	if (execute_and_wait(store, cmd, *in_fd, *out_fd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	handle_pipe_fds(in_fd, pipe_fds, is_last_cmd);
	return (EXIT_SUCCESS);
}
