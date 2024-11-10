/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:11:23 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/10 19:51:49 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

/**
 * @brief Generates a unique filename for heredoc temporary file
 *
 * @param index Sequential number to make filename unique
 * @return char* Newly allocated string containing filename
 * @note Creates filename in /tmp directory with format "heredoc_tmp_[index]"
 */
static char	*get_heredoc_filename(int index)
{
	char	*filename;
	char	*index_str;

	index_str = ft_itoa(index);
	filename = ft_strjoin("/tmp/heredoc_tmp_", index_str);
	free(index_str);
	return (filename);
}

/**
 * @brief Opens a heredoc file with specified flags
 *
 * @param filename Path to the heredoc file
 * @param flags Open flags (O_CREAT | O_WRONLY | O_TRUNC or O_RDONLY)
 * @return int File descriptor if successful, -1 on error
 * @note Creates file with 0600 permissions if O_CREAT flag is used
 */
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

/**
 * @brief Writes a line to the heredoc file
 *
 * @param fd File descriptor to write to
 * @param line String to write
 * @note Appends a newline character after the line
 */
static void	write_heredoc_line(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

/**
 * @brief Reads input lines until delimiter is encountered
 *
 * @param fd File descriptor to write input to
 * @param delimiter String that marks end of heredoc input
 * @return int 0 on success
 * @note Reads from stdin using readline, writes to fd until delimiter is found
 */
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

/**
 * @brief Handles the complete heredoc process
 *
 * @param cmd Command structure containing heredoc information
 * @return int 0 on success, 1 on error
 * @note Creates temporary file, reads input until delimiter,
 *       then opens file for reading and stores fd in cmd structure
 */
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
