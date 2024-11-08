/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:26:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/08 13:50:22 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

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
	return (EXIT_SUCCESS);
}

void	run_cmd(t_cmd *cmd, t_shell *store)
{
	if (check_builtin(cmd->command) == 0)
	{
		g_exit_status = executor(store, cmd);
		exit(g_exit_status);
	}
	else
	{
		g_exit_status = builtin_main(store, cmd->command);
		free_all(store);
		exit(g_exit_status);
	}
}

static void	handle_child_process(t_shell *store, t_cmd *cmd,
	int in_fd, int out_fd)
{
	signal(SIGINT, SIG_DFL);
	g_exit_status = redir_handler(cmd, cmd->redir, NULL);
	if (g_exit_status != 0)
		exit(g_exit_status);
	if (cmd->prev == NULL)
	{
		g_exit_status = handle_all_heredocs(store);
		if (g_exit_status != 0)
			exit(g_exit_status);
	}
	setup_pipes(in_fd, out_fd, cmd);
	run_cmd(cmd, store);
}

int	execute_command(t_shell *store, t_cmd *cmd, int in_fd, int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		print_error("Fork failed", strerror(errno));
		return (-1);
	}
	if (pid == 0)
		handle_child_process(store, cmd, in_fd, out_fd);
	signal(SIGINT, SIG_IGN);
	cmd->pid = pid;
	return (pid);
}

int	execute_and_wait(t_shell *store, t_cmd *cmd, int in_fd, int out_fd)
{
	pid_t	last_pid;

	last_pid = execute_command(store, cmd, in_fd, out_fd);
	if (last_pid == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
