/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:26:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/07 22:50:19 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

int	execute_and_wait(t_shell *store, t_cmd *cmd, int in_fd, int out_fd)
{
	pid_t	last_pid;

	last_pid = execute_command(store, cmd, in_fd, out_fd);
	if (last_pid == -1)
		return (EXIT_FAILURE);
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

static void	update_exit_status(int res)
{
	if (WIFEXITED(res))
		g_exit_status = WEXITSTATUS(res);
	else if (WIFSIGNALED(res))
		g_exit_status = WTERMSIG(res) + 128;
}

static int	wait_for_processes(t_cmd *cmd)
{
	int		res;

	res = 0;
	while ((waitpid(cmd->pid, &res, 0)) != -1)
	{
		update_exit_status(res);
		if (cmd->next)
			cmd = cmd->next;
	}
	signal(SIGINT, ctrl_c_handler);
	return (0);
}

int	multi_executor(t_shell *store)
{
	int		in_fd;
	t_cmd	*cmd;
	int		out_fd;

	in_fd = STDIN_FILENO;
	cmd = store->cmd_head;
	while (cmd)
	{
		if (handle_command(store, cmd, &in_fd, &out_fd) == EXIT_FAILURE)
		{
			if (!cmd->next)
				return (EXIT_FAILURE);
		}
		cmd = cmd->next;
	}
	return (wait_for_processes(store->cmd_head));
}

static void	process_pipe_token(t_shell *store, t_node **front,
	t_node *back, bool *create)
{
	t_node	*temp;

	temp = back->next;
	create_cmd(store, *front, back->prev, *create);
	*create = false;
	free(back->data);
	free(back);
	if (temp)
		temp->prev = NULL;
	*front = temp;
}

int	multiple_function(t_shell *store)
{
	t_node	*front;
	t_node	*back;
	bool	create;

	front = store->head;
	back = store->head;
	create = true;
	while (back->next)
	{
		if (ft_strcmp(back->data, "|") == 0)
		{
			process_pipe_token(store, &front, back, &create);
			back = front;
		}
		else
			back = back->next;
	}
	create_cmd(store, front, back, create);
	return (multi_executor(store));
}
