/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:26:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/07 22:36:54 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

static void	close_fd_if_valid(int fd)
{
	if (fd > 2)
		close(fd);
}

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

void	setup_pipes(int in_fd, int out_fd, t_cmd *cmd)
{
	setup_heredoc_fd(cmd);
	setup_input_fd(in_fd, cmd);
	setup_output_fd(out_fd, cmd);
}
