/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:26:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 14:28:35 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 
/**
 * @brief Safely closes a file descriptor if it's not a standard stream
 *
 * @param fd File descriptor to close
 * @note Only closes fd if it's greater than 2 (not stdin/stdout/stderr)
 */
static void	close_fd_if_valid(int fd)
{
	if (fd > 2)
		close(fd);
}

/**
 * @brief Sets up heredoc input redirection
 *
 * @param cmd Command structure containing heredoc file descriptor
 * @note Duplicates heredoc fd to stdin and closes original if present
 */
static void	setup_heredoc_fd(t_cmd *cmd)
{
	if (cmd->heredoc_fd != -1)
	{
		if (dup2(cmd->heredoc_fd, STDIN_FILENO) == -1)
			print_error("dup2 failed on heredoc input", strerror(errno));
		close_fd_if_valid(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
}

/**
 * @brief Sets up input file descriptor for piped commands
 *
 * @param in_fd Input file descriptor from pipe
 * @param cmd Command structure containing redirection info
 * @note Handles both pipe input and redirected input
 */
static void	setup_input_fd(int in_fd, t_cmd *cmd)
{
	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			print_error("dup2 failed on input", strerror(errno));
		close_fd_if_valid(in_fd);
	}
	if (cmd->redir && cmd->input_fd != STDIN_FILENO)
	{
		if (dup2(cmd->input_fd, STDIN_FILENO) == -1)
			print_error("dup2 failed on redirected input", strerror(errno));
		close_fd_if_valid(cmd->input_fd);
	}
}

/**
 * @brief Sets up output file descriptor for piped commands
 *
 * @param out_fd Output file descriptor from pipe
 * @param cmd Command structure containing redirection info
 * @note Handles both pipe output and redirected output
 */
static void	setup_output_fd(int out_fd, t_cmd *cmd)
{
	if (cmd->redir && cmd->output_fd != STDOUT_FILENO)
	{
		if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
			print_error("dup2 failed on redirected output", strerror(errno));
		close_fd_if_valid(cmd->output_fd);
	}
	else if (out_fd != STDOUT_FILENO)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			print_error("dup2 failed on output", strerror(errno));
		close_fd_if_valid(out_fd);
	}
}

/**
 * @brief Sets up all file descriptors for a piped command
 *
 * @param in_fd Input file descriptor from pipe
 * @param out_fd Output file descriptor from pipe
 * @param cmd Command structure containing redirection info
 * @note Handles heredoc, input, and output file descriptors in sequence
 */
void	setup_pipes(int in_fd, int out_fd, t_cmd *cmd)
{
	setup_heredoc_fd(cmd);
	setup_input_fd(in_fd, cmd);
	setup_output_fd(out_fd, cmd);
}
