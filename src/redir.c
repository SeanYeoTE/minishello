/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:56:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/14 12:34:21 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*redir_handler(t_cmd *cmd, t_node *loop, t_node *end)
{
	t_node	*temp;
	int		result;
	
	temp = loop;
	while (loop != end)
	{
		if (ft_strcmp(loop->data, "<") == 0)
		{
			result = handle_input_redirection(cmd, loop->next->data);
			if (result != 0)
				return (NULL);
		}
		else if (ft_strcmp(loop->data, "<<") == 0)
		{
			result = handle_heredoc_redirection(cmd, loop->next->data);
			if (result != 0)
				return (NULL);
		}
		loop = loop->next;
	}
	loop = temp;
	while (loop != end)
	{
		if (ft_strcmp(loop->data, ">") == 0)
		{
			result = handle_output_redirection(cmd, loop->next->data);
			if (result != 0)
				return (NULL);
		}
		else if (ft_strcmp(loop->data, ">>") == 0)
		{
			result = handle_append_redirection(cmd, loop->next->data);
			if (result != 0)
				return (NULL);
		}
		loop = loop->next;
	}
	return (loop);
}

char	*create_string(char *first, char *second, char *third)
{
	char	*ret;
	
	ret = (char *)ft_calloc(sizeof(char), ft_strlen(first) + ft_strlen(second) + ft_strlen(third) + 4);

	if (!ret)
		return (NULL);
	ft_strlcat(ret, first, ft_strlen(first) + 1);
	ft_strlcat(ret, second, ft_strlen(first) + ft_strlen(second) + 1);
	ft_strlcat(ret, ": ", ft_strlen(first) + ft_strlen(second) + 3);
	ft_strlcat(ret, third, ft_strlen(first) + ft_strlen(second) + ft_strlen(third) + 3);
	ft_strlcat(ret, "\n", ft_strlen(first) + ft_strlen(second) + ft_strlen(third) + 4);
	return (ret);
}

int	handle_output_redirection(t_cmd *cmd, char *filename)
{
	int	outputfd;
	
	outputfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outputfd == -1)
	{
		ft_putstr_fd(create_string("bash: ", filename, strerror(errno)), 2);
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
		ft_putstr_fd(create_string("bash: ", filename, strerror(errno)), 2);
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
	if (inputfd == -1)
	{
		ft_putstr_fd(create_string("bash: ", filename, strerror(errno)), 2);
		return (1);
	}
	if (cmd->input_fd != STDIN_FILENO)
		close(cmd->input_fd);
	cmd->input_fd = inputfd;
	return (0);
}

int	handle_heredoc_redirection(t_cmd *cmd, char *delimiter)
{
	cmd->heredoc_delimiter = ft_strdup(delimiter);
	if (!cmd->heredoc_delimiter)
	{
		ft_putstr_fd("Error: Memory allocation failed for heredoc delimiter\n", 2);
		return (1);
	}
	return (handle_heredoc(cmd));
}
