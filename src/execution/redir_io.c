/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_io.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 21:54:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/07 22:02:35 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

int	handle_output_redirection(t_cmd *cmd, char *filename)
{
	int	outputfd;

	outputfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outputfd == -1)
	{
		print_erroronly(strerror(errno), filename);
		cmd->output_fd = 1;
		return (1);
	}
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
	cmd->output_fd = outputfd;
	return (0);
}

int	handle_append_redirection(t_cmd *cmd, char *filename)
{
	int	outputfd;

	outputfd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (outputfd == -1)
	{
		print_erroronly(strerror(errno), filename);
		cmd->output_fd = 1;
		return (1);
	}
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
	cmd->output_fd = outputfd;
	return (0);
}

int	handle_input_redirection(t_cmd *cmd, char *filename)
{
	int	inputfd;

	inputfd = open(filename, O_RDONLY);
	cmd->input_changed = true;
	if (inputfd == -1)
	{
		print_erroronly("No such file or directory", filename);
		return (1);
	}
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	cmd->input_fd = inputfd;
	return (0);
}
