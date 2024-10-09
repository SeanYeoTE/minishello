/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:11:23 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/09 20:20:50 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"
 
static char	*get_heredoc_filename(int index)
{
	char	*filename;
	char	*index_str;

	index_str = ft_itoa(index);
	filename = ft_strjoin("/tmp/heredoc_tmp_", index_str);
	free(index_str);
	return (filename);
}


static int	open_heredoc_file(char *filename, int flags)
{
	int	fd;
	
	fd = open(filename, flags, 0600);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	return (fd);
}

static void	write_heredoc_line(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

static void	read_heredoc_input(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		write_heredoc_line(fd, line);
		free(line);
	}
}

static void	handle_nested_heredoc(t_cmd *cmd, int *index)
{
	char	*filename;
	int		fd;

	filename = get_heredoc_filename(*index);
	fd = open_heredoc_file(filename, O_CREAT | O_WRONLY | O_TRUNC);
	read_heredoc_input(fd, cmd->heredoc_delimiter);
	close(fd);
	cmd->heredoc_fd = open_heredoc_file(filename, O_RDONLY);
	free(filename);
}

void	handle_heredoc(t_cmd *cmd)
{
	static int	index = 0;
	char		*filename;
	int			fd;

	if (cmd->next && cmd->next->heredoc_delimiter)
		handle_nested_heredoc(cmd, &index);
	filename = get_heredoc_filename(index);
	fd = open_heredoc_file(filename, O_CREAT | O_WRONLY | O_TRUNC);
	read_heredoc_input(fd, cmd->heredoc_delimiter);
	close(fd);
	cmd->heredoc_fd = open_heredoc_file(filename, O_RDONLY);
	free(filename);
}

void	handle_heredoc_pipe(t_cmd *cmd)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	handle_heredoc(cmd);
	dup2(cmd->heredoc_fd, pipefd[0]);
	close(cmd->heredoc_fd);
	cmd->heredoc_fd = pipefd[0];
	cmd->pipe_out = pipefd[1];
 }