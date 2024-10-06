/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:56:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/02 13:35:19 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*redir_handler(t_cmd *cmd, t_node *loop, t_node *end)
{
	t_node	*temp;
	
	temp = loop;
	while (loop != end)
	{
		if (ft_strcmp(loop->data, ">") == 0)
			handle_output_redirection(cmd, loop->next->data);
		else if (ft_strcmp(loop->data, ">>") == 0)
			handle_append_redirection(cmd, loop->next->data);
		else if (ft_strcmp(loop->data, "<") == 0)
			handle_input_redirection(cmd, loop->next->data);
		else if (ft_strcmp(loop->data, "<<") == 0)
			handle_heredoc_redirection(cmd, loop->next->data);
		loop = loop->next;
	}
	return (loop);
}

void	handle_output_redirection(t_cmd *cmd, char *filename)
{
	int	outputfd;
	
	outputfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outputfd == -1)
	{
		print_error("Error opening output file", strerror(errno));
		return;
	}
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
	cmd->output_fd = outputfd;
}

void	handle_append_redirection(t_cmd *cmd, char *filename)
{
	int	outputfd;
	
	outputfd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (outputfd == -1)
	{
		print_error("Error opening output file for append", strerror(errno));
		return;
	}
	if (cmd->output_fd != STDOUT_FILENO)
		close(cmd->output_fd);
	cmd->output_fd = outputfd;
}

void	handle_input_redirection(t_cmd *cmd, char *filename)
{
	int	inputfd;
	
	inputfd = open(filename, O_RDONLY);
	if (inputfd == -1)
	{
		print_error("Error opening input file", strerror(errno));
		return;
	}
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	cmd->input_fd = inputfd;
}

void	handle_heredoc_redirection(t_cmd *cmd, char *delimiter)
{
	int heredoc_fd;

	cmd->heredoc_delimiter = ft_strdup(delimiter);
	handle_heredoc(cmd);
	// if (cmd->heredoc_fd != -1)
	// {
	// 	close(cmd->heredoc_fd);
	// 	cmd->heredoc_fd = -1;
	// }
	// heredoc_fd = handle_heredoc(cmd);
	// if (heredoc_fd == -1)
	// {
	// 	print_error("Failed to create heredoc", strerror(errno));
	// 	return;
	// }
	// cmd->heredoc_fd = heredoc_fd;
	// if (cmd->heredoc_delimiter)
	// 	free(cmd->heredoc_delimiter);
	// cmd->heredoc_delimiter = ft_strdup(delimiter);
}