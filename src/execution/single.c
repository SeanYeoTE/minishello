/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:38:43 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/13 15:09:20 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 
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
	redir_handler(store, cmd, cmd->redir, NULL);
	
	if (store->exit_status == 0)
	{
		signal(SIGINT, SIG_IGN);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), EXIT_FAILURE);
		if (pid == 0)
		{
			// signal(SIGINT, SIG_DFL);
			// signal(SIGQUIT, SIG_DFL);
			exit(heredoc_finisher(cmd, store));
		}
		waitpid(pid, &store->exit_status, 0);
		signal(SIGINT, ctrl_c_handler);
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
