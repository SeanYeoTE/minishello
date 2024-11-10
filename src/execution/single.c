/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:38:43 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/10 20:53:34 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

/**
 * @brief Processes heredoc redirections for a command
 *
 * @param cmd Command structure containing redirection information
 * @return int 0 on success, 1 on error
 * @note Searches for "<<" tokens and sets up heredoc with specified delimiter
 *       Allocates memory for delimiter which must be freed by caller
 */
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

/**
 * @brief Executes an external command in a child process
 *
 * @param store Shell data structure
 * @param cmd Command to execute
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error
 * @note Handles signal setup, redirections, and heredoc processing
 *       Child process exits with appropriate status
 */
int	execute_external_command(t_shell *store, t_cmd *cmd)
{
	pid_t	pid;

	pid = 0;
	pid = fork();
	if (pid == -1)
		return (perror("fork"), EXIT_FAILURE);
	signal(SIGINT, SIG_IGN);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		store->exit_status = redir_handler(cmd, cmd->redir, NULL);
		if (store->exit_status != 0)
		{
			free_all(store);
			exit(store->exit_status);
		}
		heredoc_finisher(cmd);
		if (cmd->command == NULL)
		{
			free_all(store);
			exit(store->exit_status);
		}
		store->exit_status = executor(store, cmd);
		exit(store->exit_status);
	}
	return (wait_for_command(store, pid));
}

/**
 * @brief Sets up file descriptors for builtin command execution
 *
 * @param cmd Command structure containing redirection information
 * @note Handles both input and output redirection
 *       Closes original file descriptors after duplication
 */
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

/**
 * @brief Executes a builtin command with proper redirection
 *
 * @param store Shell data structure
 * @param cmd Command to execute
 * @return int Exit status of the builtin command
 * @note Handles redirections, heredoc processing, and signal management
 *       Resets file descriptors after command execution
 */
int	execute_builtin_command(t_shell *store, t_cmd *cmd)
{
	pid_t	pid;
	store->exit_status = redir_handler(cmd, cmd->redir, NULL);
	if (store->exit_status == 0)
	{
		pid = fork();
		if (pid == -1)
			return (perror("fork"), EXIT_FAILURE);
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			heredoc_finisher(cmd);
		}
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		waitpid(pid, &store->exit_status, 0);
		set_builtin_fd(cmd);
		store->exit_status = builtin_main(store, cmd->command);
		reset_fds(store);
	}
	return (store->exit_status);
}

/**
 * @brief Handles execution of a single command (no pipes)
 *
 * @param store Shell data structure
 * @param head Start of command token list
 * @param tail End of command token list
 * @return int Exit status of command execution
 * @note Creates command structure and executes either builtin or external command
 *       Handles syntax errors and empty commands with redirections
 */
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
