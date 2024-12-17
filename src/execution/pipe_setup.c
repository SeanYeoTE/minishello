/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:26:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/13 15:16:04 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 
/**
 * @brief Counts the number of pipe tokens in a command list
 *
 * @param loop Pointer to the first node in the command list
 * @return int Number of pipe tokens found
 * @note Traverses the linked list and counts occurrences of "|" tokens
 */
int	pipe_counter(t_node *loop)
{
	int	count;

	count = 0;
	while (loop)
	{
		if (ft_strcmp(loop->data, "|") == 0)
			count++;
		loop = loop->next;
	}
	return (count);
}

/**
 * @brief Creates a new pipe and sets up its file descriptors
 *
 * @param pipe_fds Array to store read (pipe_fds[0]) and write (pipe_fds[1]) ends
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE if pipe creation fails
 * @note Prints error message if pipe creation fails
 */
int	setup_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
	{
		print_error("Pipe failed", strerror(errno));
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Manages pipe file descriptors between commands
 *
 * @param in_fd Pointer to input file descriptor
 * @param pipe_fds Array containing pipe read/write file descriptors
 * @param is_last_cmd Flag indicating if this is the last command in pipeline
 * @note Closes unused file descriptors and sets up input for next command
 */
void	handle_pipe_fds(int *in_fd, int pipe_fds[2], int is_last_cmd)
{
	if (*in_fd > 2)
		close(*in_fd);
	if (!is_last_cmd)
	{
		if (pipe_fds[1] > 2)
			close(pipe_fds[1]);
		*in_fd = pipe_fds[0];
	}
}

/**
 * @brief Waits for a command to complete and updates shell status
 *
 * @param store Shell data structure to update exit status
 * @param pid Process ID to wait for
 * @return int EXIT_SUCCESS after command completion
 * @note Handles both normal exit and signal termination cases
 */
int	wait_for_command(t_shell *store, pid_t pid)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 0);
	signal(SIGINT, ctrl_c_handler);
	if (WIFEXITED(status))
		store->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		store->exit_status = WTERMSIG(status) + 128;
	return (EXIT_SUCCESS);
}

/**
 * @brief Sets up and executes a command in a pipeline
 *
 * @param store Shell data structure
 * @param cmd Current command to execute
 * @param in_fd Pointer to input file descriptor
 * @param out_fd Pointer to output file descriptor
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error
 * @note Creates pipes if needed, executes command, and manages file descriptors
 */
int	handle_command(t_shell *store, t_cmd *cmd, int *in_fd, int *out_fd)
{
	int		pipe_fds[2];
	int		is_last_cmd;

	is_last_cmd = (cmd->next == NULL);
	if (!is_last_cmd)
	{
		if (setup_pipe(pipe_fds) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		*out_fd = pipe_fds[1];
	}
	else
		*out_fd = STDOUT_FILENO;
	if (execute_and_wait(store, cmd, *in_fd, *out_fd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	handle_pipe_fds(in_fd, pipe_fds, is_last_cmd);
	return (EXIT_SUCCESS);
}
