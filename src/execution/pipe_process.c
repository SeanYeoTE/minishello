/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:26:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/10 20:18:48 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

/**
 * @brief Processes all heredoc inputs for commands in pipeline
 *
 * @param store Shell data structure containing command list
 * @return int EXIT_SUCCESS if all heredocs processed, EXIT_FAILURE on error
 * @note Must be called before executing pipeline to set up all heredoc inputs
 */
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

/**
 * @brief Executes a command, either builtin or external
 *
 * @param cmd Command structure to execute
 * @param store Shell data structure
 * @note Exits process with appropriate status after execution
 */
void	run_cmd(t_cmd *cmd, t_shell *store)
{
	if (check_builtin(cmd->command) == 0)
	{
		store->exit_status = executor(store, cmd);
		exit(store->exit_status);
	}
	else
	{
		store->exit_status = builtin_main(store, cmd->command);
		free_all(store);
		exit(store->exit_status);
	}
}

/**
 * @brief Handles child process setup and execution
 *
 * @param store Shell data structure
 * @param cmd Command to execute
 * @param in_fd Input file descriptor
 * @param out_fd Output file descriptor
 * @note Sets up signals, redirections, heredocs, and executes command
 */
static void	handle_child_process(t_shell *store, t_cmd *cmd,
	int in_fd, int out_fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	store->exit_status = redir_handler(cmd, cmd->redir, NULL);
	if (store->exit_status != 0)
		exit(store->exit_status);
	if (cmd->prev == NULL)
	{
		store->exit_status = handle_all_heredocs(store);
		if (store->exit_status != 0)
			exit(store->exit_status);
	}
	setup_pipes(in_fd, out_fd, cmd);
	run_cmd(cmd, store);
}

/**
 * @brief Creates a new process for command execution
 *
 * @param store Shell data structure
 * @param cmd Command to execute
 * @param in_fd Input file descriptor
 * @param out_fd Output file descriptor
 * @return int Process ID on success, -1 on failure
 * @note Parent process ignores signals while child executes
 */
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
	signal(SIGQUIT, SIG_IGN);
	cmd->pid = pid;
	return (pid);
}

/**
 * @brief Executes a command and handles process management
 *
 * @param store Shell data structure
 * @param cmd Command to execute
 * @param in_fd Input file descriptor
 * @param out_fd Output file descriptor
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error
 * @note Creates process and stores PID in command structure
 */
int	execute_and_wait(t_shell *store, t_cmd *cmd, int in_fd, int out_fd)
{
	pid_t	last_pid;

	last_pid = execute_command(store, cmd, in_fd, out_fd);
	if (last_pid == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
