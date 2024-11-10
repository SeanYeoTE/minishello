/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_io.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 21:54:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/10 20:36:22 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

/**
 * @brief Handles output redirection (>) for a command
 *
 * @param cmd Command structure to set up output redirection for
 * @param filename Name of the file to redirect output to
 * @return int 0 on success, 1 on error
 * @note Creates or truncates file with permissions 0644
 *       Closes any previously opened output file descriptor
 */
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

/**
 * @brief Handles append redirection (>>) for a command
 *
 * @param cmd Command structure to set up append redirection for
 * @param filename Name of the file to append output to
 * @return int 0 on success, 1 on error
 * @note Creates file if it doesn't exist, appends if it does
 *       Uses permissions 0644 for new files
 */
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

/**
 * @brief Handles input redirection (<) for a command
 *
 * @param cmd Command structure to set up input redirection for
 * @param filename Name of the file to read input from
 * @return int 0 on success, 1 on error
 * @note Sets input_changed flag and closes any previously opened input fd
 *       Returns error if file doesn't exist or can't be opened
 */
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
