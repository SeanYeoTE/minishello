/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:38:43 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/18 17:34:00 by seayeo           ###   ########.fr       */
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
		redir_handler(cmd, cmd->redir, NULL);
		t_exit_status = executor(store, cmd);
		exit(t_exit_status);
	}
	return (wait_for_command(pid));
}

int	execute_builtin_command(t_shell *store, t_cmd *cmd)
{
	redir_handler(cmd, cmd->redir, NULL);
	t_exit_status = builtin_main(store, cmd->command, cmd->redir);
	return (t_exit_status);
}

int	single_function(t_shell *store, t_node *head, t_node *tail)
{
	create_cmd(store, head, tail, true);
	if (check_builtin(store->cmd_head->command) == 0)
	{
		return (execute_external_command(store, store->cmd_head));
	}
	else
	{
		return (execute_builtin_command(store, store->cmd_head));
	}
	return (0);
}
