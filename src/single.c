/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:38:43 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/06 13:12:13 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_external_command(t_shell *store, t_cmd *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), EXIT_FAILURE);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		
		t_exit_status = redir_handler(cmd, cmd->redir, NULL);
		if (t_exit_status != 0)
			exit(t_exit_status);
		t_exit_status = executor(store, cmd);
		exit(t_exit_status);
	}
	signal(SIGINT, SIG_IGN);
	return (wait_for_command(pid));
}

static void	set_fd(t_cmd *cmd)
{
	if (cmd->redir && cmd->input_fd != STDIN_FILENO)
	{
		if (dup2(cmd->input_fd, STDIN_FILENO) == -1)
			perror("dup2 input");
		close(cmd->input_fd);
	}
	if (cmd->redir && cmd->output_fd != STDOUT_FILENO)
	{
		if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
			perror("dup2 output");
		close(cmd->output_fd);
	}
}

int	execute_builtin_command(t_shell *store, t_cmd *cmd)
{
	t_exit_status = redir_handler(cmd, cmd->redir, NULL);
	if (t_exit_status == 0)
	{
		set_fd(cmd);
		t_exit_status = builtin_main(store, cmd->command);
		reset_fds(store);
	}
	return (t_exit_status);
}

int	single_function(t_shell *store, t_node *head, t_node *tail)
{
	create_cmd(store, head, tail, true);
	// print_cmd_stack(&store->cmd_head);
	if (store->cmd_head->command == NULL)
	{
		print_erroronly("syntax error", "newline");
		return (2);
	}
	else if (check_builtin(store->cmd_head->command) == 0)
	{
		return (execute_external_command(store, store->cmd_head));
	}
	else
	{
		return (execute_builtin_command(store, store->cmd_head));
	}
	return (0);
}
