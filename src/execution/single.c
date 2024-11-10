/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:38:43 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/10 14:29:42 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

int	heredoc_finisher(t_cmd *cmd)
{
	t_node	*tmp;
	int		result;

	result = 0;
	tmp = cmd->redir;
	while (tmp)
	{
		if (ft_strcmp(tmp->data, "<<") == 0)
		{
			cmd->heredoc_delimiter = ft_strdup(tmp->next->data);
			if (cmd->heredoc_delimiter == NULL)
				return (1);
			result = handle_heredoc(cmd);
		}
		tmp = tmp->next;
	}
	return (result);
}

int	execute_external_command(t_shell *store, t_cmd *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), EXIT_FAILURE);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		g_exit_status = redir_handler(cmd, cmd->redir, NULL);
		if (g_exit_status != 0)
		{
			free_all(store);
			exit(g_exit_status);
		}
		heredoc_finisher(cmd);
		if (cmd->command == NULL)
		{
			free_all(store);
			exit(g_exit_status);
		}
		g_exit_status = executor(store, cmd);
		exit(g_exit_status);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	return (wait_for_command(pid));
}

static void	set_builtin_fd(t_cmd *cmd)
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
	g_exit_status = redir_handler(cmd, cmd->redir, NULL);
	if (g_exit_status == 0)
	{
		heredoc_finisher(cmd);
		set_builtin_fd(cmd);
		g_exit_status = builtin_main(store, cmd->command);
		reset_fds(store);
	}
	return (g_exit_status);
}

int	single_function(t_shell *store, t_node *head, t_node *tail)
{
	create_cmd(store, head, tail, true);
	if (store->cmd_head->command == NULL && store->cmd_head->redir == NULL)
	{
		print_erroronly("syntax error", "newline");
		return (2);
	}
	else if (store->cmd_head->command == NULL && store->cmd_head->redir)
		return (execute_external_command(store, store->cmd_head));
	else if (check_builtin(store->cmd_head->command) == 0)
		return (execute_external_command(store, store->cmd_head));
	else
		return (execute_builtin_command(store, store->cmd_head));
	return (0);
}
