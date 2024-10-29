/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:56:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/29 22:33:41 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int are_same_resource(int fd1, int fd2) {
    struct stat stat1, stat2;

    // Get stats for the first file descriptor
    if (fstat(fd1, &stat1) == -1) {
        perror("fstat fd1");
        return 0;  // Error occurred
    }

    // Get stats for the second file descriptor
    if (fstat(fd2, &stat2) == -1) {
        perror("fstat fd2");
        return 0;  // Error occurred
    }

    // Compare device and inode numbers
    return (stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino);
}

void	reset_fds(t_shell *store, t_cmd *cmd)
{
	if (!are_same_resource(store->input_fd, STDIN_FILENO))
	{
		if (dup2(store->input_fd, STDIN_FILENO) == -1)
			perror("dup2");
	}
	if (!are_same_resource(store->output_fd, STDOUT_FILENO))
	{
		if (dup2(store->output_fd, STDOUT_FILENO) == -1)
			perror("dup2");
	}
	if (cmd->heredoc_fd > 2)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (cmd->heredoc_delimiter)
	{
		free(cmd->heredoc_delimiter);
		cmd->heredoc_delimiter = NULL;
	}
	cmd->input_changed = false;
}

int	redir_handler(t_cmd *cmd, t_node *loop, t_node *end)
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
				return (result);
		}
		else if (ft_strcmp(loop->data, "<<") == 0)
		{
			result = handle_heredoc_redirection(cmd, loop->next->data);
			if (result != 0)
				return (result);
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
				return (result);
		}
		else if (ft_strcmp(loop->data, ">>") == 0)
		{
			result = handle_append_redirection(cmd, loop->next->data);
			if (result != 0)
				return (result);
		}
		loop = loop->next;
	}
	return (0);
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
	return (ret);
}

int	handle_output_redirection(t_cmd *cmd, char *filename)
{
	int	outputfd;

	outputfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outputfd == -1)
	{
		print_erroronly(strerror(errno), filename);
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
	
	if (access(filename, F_OK) == 0)
		outputfd = open(filename, O_WRONLY | O_APPEND);
	else
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
