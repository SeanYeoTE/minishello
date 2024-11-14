/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:11:23 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/15 01:13:48 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 

/**
 * @brief Writes a line to the heredoc pipe
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
static int	read_heredoc_input(int fd, char *delimiter, t_shell *store)
{
	char	*line;

	(void)store;
	signal(SIGINT, child_sigint_handler);
	g_sig = 0;  // Reset signal flag at start
	rl_outstream = stderr;  // Redirect readline's output to stderr
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
 * @brief Handles the complete heredoc process using pipes
 *
 * @param cmd Command structure containing heredoc information
 * @return int 0 on success, 1 on error
 * @note Uses existing pipe file descriptors from cmd structure
 */
int	heredoc_single_external(t_cmd *cmd, t_shell* store)
{
	int		result;

	result = read_heredoc_input(cmd->heredoc_write_fd, cmd->heredoc_delimiter, store);
	close(cmd->heredoc_write_fd);  // Close write end

	if (result != 0)  // If interrupted
	{
		close(cmd->heredoc_fd);  // Close read end
		return (130);  // Return 130 for SIGINT
	}
	
	return (0);
}
