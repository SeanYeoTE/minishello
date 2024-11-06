/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:56:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/06 23:51:54 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

bool are_same_resource(int fd1, int fd2) {
    struct stat stat1, stat2;

    // Get file status for fd1
    if (fstat(fd1, &stat1) == -1) {
        perror("fstat");
        return false;
    }

    // Get file status for fd2
    if (fstat(fd2, &stat2) == -1) {
        perror("fstat");
        return false;
    }

    // Compare device and inode
    return (stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino);
}

void	reset_fds(t_shell *store)
{
	if (!are_same_resource(store->input_reset, STDIN_FILENO))
	{
		if (dup2(store->input_reset, STDIN_FILENO) == -1)
			print_erroronly("dup2 failed on input reset", strerror(errno));
	}
	if (!are_same_resource(store->output_reset, STDOUT_FILENO))
	{
		if (dup2(store->output_reset, STDOUT_FILENO) == -1)
			print_erroronly("dup2 failed on output reset", strerror(errno));
	}
}

int	redir_handler(t_cmd *cmd, t_node *loop, t_node *end)
{
	int		result;

	result = 0;
	while (loop != end)
	{
		if (ft_strcmp(loop->data, "<") == 0)
		{
			result = handle_input_redirection(cmd, loop->next->data);
			if (result != 0)
				break ;
		}
		// else if (ft_strcmp(loop->data, "<<") == 0)
		// {
		// 	result = handle_heredoc_redirection(cmd, loop->next->data);
		// 	if (result != 0)
		// 		break ;
		// }
		else if (ft_strcmp(loop->data, ">") == 0)
		{
			result = handle_output_redirection(cmd, loop->next->data);
			if (result != 0)
				break ;
		}
		else if (ft_strcmp(loop->data, ">>") == 0)
		{
			result = handle_append_redirection(cmd, loop->next->data);
			if (result != 0)
				break ;
		}
		loop = loop->next;
	}
	return (result);
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
		ft_putstr_fd(create_string("bash: ", filename, strerror(errno)), 2);
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

// int	handle_heredoc_redirection(t_cmd *cmd, char *delimiter)
// {
// 	cmd->heredoc_delimiter = ft_strdup(delimiter);
// 	if (!cmd->heredoc_delimiter)
// 	{
// 		ft_putstr_fd("Error: Memory allocation failed for heredoc delimiter\n", 2);
// 		return (1);
// 	}
// 	return (0); // Just set up the delimiter, actual heredoc handling happens later
// }