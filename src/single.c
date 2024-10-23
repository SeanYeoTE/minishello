/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:38:43 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/23 15:12:15 by seayeo           ###   ########.fr       */
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
		t_exit_status = redir_handler(cmd, cmd->redir, NULL);
		if (t_exit_status != 0)
			exit(t_exit_status);
		t_exit_status = executor(store, cmd, 0);
		exit(t_exit_status);
	}
	return (wait_for_command(pid));
}

int	execute_builtin_command(t_shell *store, t_cmd *cmd)
{
	t_exit_status = redir_handler(cmd, cmd->redir, NULL);
	if (t_exit_status == 0)
		t_exit_status = builtin_main(store, cmd->command, cmd->redir);
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
