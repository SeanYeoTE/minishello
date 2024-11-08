/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/07 22:23:12 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

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

static void	handle_output_fd(t_cmd *node)
{
	if (node->output_fd != STDOUT_FILENO)
	{
		if (dup2(node->output_fd, STDOUT_FILENO) == -1)
			print_error("dup2 failed on output", strerror(errno));
		close(node->output_fd);
	}
}

void	set_fd(t_cmd *node, char *temp_filename)
{
	handle_heredoc_fd(node);
	handle_input_fd(node, temp_filename);
	handle_output_fd(node);
}

void	check_open_fds(int max_fd)
{
	int	fd;

	fd = 3;
	while (fd <= max_fd)
	{
		if (fcntl(fd, F_GETFD) != -1 || errno != EBADF)
			close(fd);
		fd++;
	}
}
