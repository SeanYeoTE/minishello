/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:11:23 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/14 16:39:50 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 
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
 * @return int 0 on success, 1 on interrupt
 * @note Reads from stdin using readline, writes to fd until delimiter is found
 */
static int	read_heredoc_input(int fd, char *delimiter, char *filename, t_shell *store)
{
	char	*line;

	(void)filename;
	(void)store;
	signal(SIGINT, child_sigint_handler);
	g_sig = 0;  // Reset signal flag at start
	while (1)
	{
		line = readline("> ");
		if (g_sig == 1)
		{
			if (line)
				free(line);
			return (1);  // Return 1 to indicate interrupt
		}
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			return (0);  // Return 0 for normal completion
		}
		write_heredoc_line(fd, line);
		free(line);
	}
}

/**
 * @brief Handles the complete heredoc process
 *
 * @param cmd Command structure containing heredoc information
 * @return int 0 on success, 1 on error
 * @note Creates temporary file, reads input until delimiter,
 *       then stores filename in cmd structure for later use
 */
int	handle_heredoc(t_cmd *cmd, t_shell* store)
{
	static int	index;
	int			fd;
	int			result;

	index = 0;
	cmd->heredoc_filename = get_heredoc_filename(index++);
	if (!cmd->heredoc_filename)
		return (1);
	fd = open_heredoc_file(cmd->heredoc_filename, O_CREAT | O_WRONLY | O_TRUNC);
	if (fd == -1)
		return (free(cmd->heredoc_filename), 1);
	result = read_heredoc_input(fd, cmd->heredoc_delimiter, cmd->heredoc_filename, store);
	close(fd);
	if (result != 0)  // If interrupted
	{
		unlink(cmd->heredoc_filename);  // Remove the temporary file
		free(cmd->heredoc_filename);
		cmd->heredoc_filename = NULL;
		free_all(store);
		return (130);  // Return 130 for SIGINT
	}
	free_all(store);
	return (0);
}
