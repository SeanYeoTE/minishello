/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_core.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:11:23 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 18:35:12 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 

int	expandforheredoc(t_cmd *cmd)
{
	char	*delimiter;
	int		i;
	int		in_single_quotes;
	int		in_double_quotes;

	delimiter = cmd->heredoc_delimiter;
	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (delimiter[i])
	{
		if (delimiter[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (delimiter[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		i++;
	}
	if (in_single_quotes)
		return (0);
	return (1);
}

/**
 * @brief Writes a line to the heredoc pipe
 * @param fd File descriptor to write to
 * @param line String to write
 * @param should_write Flag indicating if we should write to pipe
 * @param expand Flag indicating if variables should be expanded
 * @note Appends a newline character after the line
 */
static void	write_heredoc_line(t_cmd *cmd, char *line, int should_write,
				t_shell *store)
{
	char	*expanded_line;

	if (should_write)
	{
		expanded_line = line;
		if (expandforheredoc(cmd))
			expanded_line = expansions(store, line);
		write(cmd->heredoc_write_fd, expanded_line, ft_strlen(expanded_line));
		write(cmd->heredoc_write_fd, "\n", 1);
		if (expandforheredoc(cmd) && expanded_line != line)
			free(expanded_line);
		else
			free(line);
	}
}

/**
 * @brief Reads input lines until delimiter is encountered
 *
 * @param fd File descriptor to write input to
 * @param delimiter String that marks end of heredoc input
 * @param should_write Flag indicating if we should write to pipe
 * @return int 0 on success, 1 on interrupt
 * @note Reads from stdin using readline, writes to fd until delimiter is found
 */
static int	read_heredoc_input(t_cmd *cmd, t_shell *store, int should_write)
{
	char	*line;

	(void)store;
	signal(SIGINT, child_sigint_handler);
	g_sig = 0;
	rl_outstream = stderr;
	while (1)
	{
		line = readline("> ");
		if (g_sig == 1)
		{
			if (line)
				free(line);
			return (1);
		}
		if (!line || ft_strcmp(line, cmd->heredoc_delimiter) == 0)
		{
			if (line)
				free(line);
			return (0);
		}
		write_heredoc_line(cmd, line, should_write, store);
	}
}

/**
 * @brief Handles the complete heredoc process using pipes
 *
 * @param cmd Command structure containing heredoc information
 * @param is_last_heredoc Flag indicating if this is the last heredoc
 * @return int 0 on success, 1 on error
 * @note Uses existing pipe file descriptors from cmd structure
 */
int	exec_heredoc(t_cmd *cmd, t_shell *store, int is_last_heredoc)
{
	int		result;

	result = read_heredoc_input(cmd, store, is_last_heredoc);
	if (result != 0)
	{
		close(cmd->heredoc_write_fd);
		close(cmd->heredoc_fd);
		return (130);
	}
	return (0);
}

/**
 * @brief Processes heredoc redirections for a command
 *
 * @param cmd Command structure containing redirection information
 * @return int 0 on success, 1 on error
 * @note Searches for "<<" tokens and sets up heredoc with specified delimiter
 *       Allocates memory for delimiter which must be freed by caller
 */
int	heredoc_finisher(t_cmd *cmd, t_shell *store)
{
	t_node	*tmp;
	int		result;
	int		last_heredoc;

	result = 0;
	tmp = cmd->redir;
	while (tmp)
	{
		if (ft_strcmp(tmp->data, "<<") == 0)
		{
			last_heredoc = is_last_heredoc(tmp);
			if (cmd->heredoc_delimiter)
				free(cmd->heredoc_delimiter);
			cmd->heredoc_delimiter = ft_strdup(tmp->next->data);
			if (cmd->heredoc_delimiter == NULL)
				return (1);
			result = exec_heredoc(cmd, store, last_heredoc);
			if (result != EXIT_SUCCESS)
				break ;
		}
		tmp = tmp->next;
	}
	close_heredoc_write(cmd);
	return (result);
}
