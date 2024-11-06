/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:05:29 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/07 00:02:38 by seayeo           ###   ########.fr       */
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

int	wait_for_command(pid_t pid)
{
	int	status;
	
	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		t_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		t_exit_status = WTERMSIG(status) + 128;
	signal(SIGINT, ctrl_c_handler);
	return (EXIT_SUCCESS);
}

static int	handle_all_heredocs(t_shell *store)
{
	t_cmd	*cmd;
	int		result;

	result = 0;
	cmd = store->cmd_head;
	while (cmd)
	{
		result = heredoc_finisher(cmd);
		if (result != 0)
			return (EXIT_FAILURE);
		cmd = cmd->next;
	}
	return EXIT_SUCCESS;
}

void	run_cmd(t_cmd *cmd, t_shell *store)
{
	if (check_builtin(cmd->command) == 0)
	{
		t_exit_status = executor(store, cmd);
		exit(t_exit_status);
	}
	else
	{
		t_exit_status = builtin_main(store, cmd->command);
		free_all(store);
		exit(t_exit_status);
	}
}

static void	close_fd_if_valid(int fd)
{
	if (fd > 2)
		close(fd);
}

void	setup_pipes(int in_fd, int out_fd, t_cmd *cmd)
{
	if (cmd->heredoc_fd != -1)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
			print_error("dup2 failed on heredoc input", strerror(errno));
		close_fd_if_valid(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	else if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			print_error("dup2 failed on input", strerror(errno));
		close_fd_if_valid(in_fd);
	}
	if (cmd->redir && cmd->input_fd != STDIN_FILENO)
	{
		if (dup2(cmd->input_fd, STDIN_FILENO) == -1)
			print_error("dup2 failed on redirected input", strerror(errno));
		close_fd_if_valid(cmd->input_fd);
	}
	if (cmd->redir && cmd->output_fd != STDOUT_FILENO)
	{
		if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
			print_error("dup2 failed on redirected output", strerror(errno));
		close_fd_if_valid(cmd->output_fd);
	}
	else if (out_fd != STDOUT_FILENO)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			print_error("dup2 failed on output", strerror(errno));
		close_fd_if_valid(out_fd);
	}
}

int	execute_command(t_shell *store, t_cmd *cmd, int in_fd, int out_fd)
{
	pid_t	pid;
	
	pid = fork();
	if (pid < 0)
	{
		print_error("Fork failed", strerror(errno));
		return -1;
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		t_exit_status = redir_handler(cmd, cmd->redir, NULL);
		if (t_exit_status != 0)
			exit(t_exit_status);
		if (cmd->prev == NULL)
		{
			t_exit_status = handle_all_heredocs(store);
			if (t_exit_status != 0)
				exit(t_exit_status);
		}
		setup_pipes(in_fd, out_fd, cmd);
		run_cmd(cmd, store);
	}
	signal(SIGINT, SIG_IGN);
	cmd->pid = pid;
	return pid;
}

int	setup_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
	{
		print_error("Pipe failed", strerror(errno));
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void	handle_pipe_fds(int *in_fd, int pipe_fds[2], int is_last_cmd)
{
	close_fd_if_valid(*in_fd);
	if (!is_last_cmd)
	{
		close_fd_if_valid(pipe_fds[1]);
		*in_fd = pipe_fds[0];
	}
	else
	{
		close_fd_if_valid(pipe_fds[0]);
		close_fd_if_valid(pipe_fds[1]);
	}
}

int	execute_and_wait(t_shell *store, t_cmd *cmd, int in_fd, int out_fd)
{
	pid_t	last_pid;

	// if (cmd->heredoc_delimiter)
	// 	handle_heredoc_pipe(cmd);
	last_pid = execute_command(store, cmd, in_fd, out_fd);
	if (last_pid == -1)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int	handle_command(t_shell *store, t_cmd *cmd, int *in_fd, int *out_fd)
{
	int		pipe_fds[2];
	int		is_last_cmd;

	is_last_cmd = (cmd->next == NULL);
	if (!is_last_cmd)
	{
		if (setup_pipe(pipe_fds) == EXIT_FAILURE)
			return EXIT_FAILURE;
		*out_fd = pipe_fds[1];
	}
	else
		*out_fd = STDOUT_FILENO;
	if (execute_and_wait(store, cmd, *in_fd, *out_fd) == EXIT_FAILURE)
		return EXIT_FAILURE;
	handle_pipe_fds(in_fd, pipe_fds, is_last_cmd);
	return EXIT_SUCCESS;
}

int	multi_executor(t_shell *store)
{
	int		in_fd;
	t_cmd	*cmd;
	int		out_fd;
	int		res;

	

	in_fd = STDIN_FILENO;
	cmd = store->cmd_head;
	while (cmd)
	{
		if (handle_command(store, cmd, &in_fd, &out_fd) == EXIT_FAILURE)
		{
			if (!cmd->next)
				return EXIT_FAILURE;
		}
		cmd = cmd->next;
	}
	res = 0;
	cmd = store->cmd_head;
	while ((waitpid(cmd->pid, &res, 0)) != -1)
	{
		if (WIFEXITED(res))
			t_exit_status = WEXITSTATUS(res);
		else if (WIFSIGNALED(res))
			t_exit_status = WTERMSIG(res) + 128;
		if (cmd->next)
			cmd = cmd->next;
	}
	signal(SIGINT, ctrl_c_handler);
	return (0);
}
