/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:11:23 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/07 21:50:36 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

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
		return (-1);
	}
	return (fd);
}

static void	write_heredoc_line(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

static int	read_heredoc_input(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(fd, line);
		free(line);
	}
	return (0);
}

int	handle_heredoc(t_cmd *cmd)
{
	static int	index = 0;
	char		*filename;
	int			fd;

	filename = get_heredoc_filename(index);
	fd = open_heredoc_file(filename, O_CREAT | O_WRONLY | O_TRUNC);
	if (fd == -1)
		return (free(filename), 1);
	if (read_heredoc_input(fd, cmd->heredoc_delimiter) != 0)
	{
		close(fd);
		return (free(filename), 1);
	}
	close(fd);
	cmd->heredoc_fd = open_heredoc_file(filename, O_RDONLY);
	if (cmd->heredoc_fd == -1)
	{
		free(filename);
		return (1);
	}
	free(filename);
	return (0);
}
