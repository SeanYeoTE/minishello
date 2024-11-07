/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:26:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/07 22:43:55 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

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
