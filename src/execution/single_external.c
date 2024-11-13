/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_external.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 13:56:06 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/13 15:14:40 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 
/**
 * @brief Sets up signal handlers for child process
 */
static void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/**
 * @brief Handles redirection and heredoc setup in child process
 * 
 * @param store Shell data structure
 * @param cmd Command to execute
 * @return int EXIT_SUCCESS on success, error status on failure
 */
static int	setup_child_io(t_shell *store, t_cmd *cmd)
{
	redir_handler(store, cmd, cmd->redir, NULL);
	if (store->exit_status != 0)
		return (store->exit_status);
	
	if (heredoc_finisher(cmd, store) != 0)
		return (EXIT_FAILURE);
		
	if (cmd->command == NULL)
		return (store->exit_status);
		
	return (EXIT_SUCCESS);
}

/**
 * @brief Executes command in child process and handles cleanup
 * 
 * @param store Shell data structure
 * @param cmd Command to execute
 */
static void	execute_child_process(t_shell *store, t_cmd *cmd)
{
	int	setup_status;
	
	setup_child_signals();
	setup_status = setup_child_io(store, cmd);
	if (setup_status != EXIT_SUCCESS)
	{
		free_all(store);
		exit(setup_status);
	}
	
	store->exit_status = executor(store, cmd);
	exit(store->exit_status);
}

/**
 * @brief Executes an external command in a child process
 *
 * @param store Shell data structure
 * @param cmd Command to execute
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error
 * @note Handles process creation, signal setup, and command execution
 */
int	execute_external_command(t_shell *store, t_cmd *cmd)
{
	pid_t	pid;

	if (!store || !cmd)
		return (EXIT_FAILURE);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (EXIT_FAILURE);
	}
	if (pid == 0)
		execute_child_process(store, cmd);
	return (wait_for_command(store, pid));
}