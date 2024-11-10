/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/10 19:49:36 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

/**
 * @brief Handles the heredoc file descriptor setup
 *
 * @param node Command node containing the heredoc file descriptor
 * @note Duplicates heredoc fd to stdin if present, then closes the original fd
 */
static void	handle_heredoc_fd(t_cmd *node)
{
	if (node->heredoc_fd != -1)
	{
		if (dup2(node->heredoc_fd, STDIN_FILENO) == -1)
			print_error("dup2 failed on heredoc input", strerror(errno));
		close(node->heredoc_fd);
		node->heredoc_fd = -1;
	}
}

/**
 * @brief Handles the input file descriptor setup
 *
 * @param node Command node containing the input file descriptor
 * @param temp_filename Name of temporary file to use as input if needed
 * @note Handles both regular input redirection and temporary file input
 */
static void	handle_input_fd(t_cmd *node, char *temp_filename)
{
	if (node->input_fd != STDIN_FILENO)
	{
		if (dup2(node->input_fd, STDIN_FILENO) == -1)
			print_error("dup2 failed on input", strerror(errno));
		close(node->input_fd);
	}
	else if (temp_filename)
	{
		node->input_fd = open(temp_filename, O_RDONLY);
		if (node->input_fd == -1)
			print_error("Failed to open temporary file", strerror(errno));
		else if (dup2(node->input_fd, STDIN_FILENO) == -1)
			print_error("dup2 failed on temporary input", strerror(errno));
	}
}

/**
 * @brief Handles the output file descriptor setup
 *
 * @param node Command node containing the output file descriptor
 * @note Duplicates output fd to stdout if different from default, then closes original
 */
static void	handle_output_fd(t_cmd *node)
{
	if (node->output_fd != STDOUT_FILENO)
	{
		if (dup2(node->output_fd, STDOUT_FILENO) == -1)
			print_error("dup2 failed on output", strerror(errno));
		close(node->output_fd);
	}
}

/**
 * @brief Sets up all file descriptors for a command
 *
 * @param node Command node containing all file descriptors
 * @param temp_filename Name of temporary file to use as input if needed
 * @note Handles heredoc, input, and output file descriptors in sequence
 */
void	set_fd(t_cmd *node, char *temp_filename)
{
	handle_heredoc_fd(node);
	handle_input_fd(node, temp_filename);
	handle_output_fd(node);
}

/**
 * @brief Closes all file descriptors above a certain number
 *
 * @param max_fd Maximum file descriptor number to check
 * @note Used to clean up any leftover file descriptors
 */
void	check_open_fds(int max_fd)
{
	int	fd;

	fd = 3;
	while (fd <= max_fd)
	{
		close(fd);
		fd++;
	}
}
