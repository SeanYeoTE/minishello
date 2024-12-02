/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 10:00:00 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 16:17:39 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Check if this is the last heredoc in the command
 * @param current Current node in redirection list
 * @return int 1 if last heredoc, 0 otherwise
 */
int	is_last_heredoc(t_node *current)
{
	t_node	*tmp;

	tmp = current->next->next;
	while (tmp)
	{
		if (ft_strcmp(tmp->data, "<<") == 0)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

/**
 * @brief Frees the heredoc delimiter if allocated
 * @param cmd Command structure containing delimiter
 */
void	cleanup_delimiter(t_cmd *cmd)
{
	if (cmd->heredoc_delimiter)
		free(cmd->heredoc_delimiter);
}

/**
 * @brief Processes a single heredoc node
 * @param cmd Command structure
 * @param store Shell data structure
 * @param node Current node in redirection list
 * @return int Result of heredoc execution
 */
int	process_heredoc_node(t_cmd *cmd, t_shell *store, t_node *node)
{
	int	last_heredoc;

	last_heredoc = is_last_heredoc(node);
	cleanup_delimiter(cmd);
	cmd->heredoc_delimiter = ft_strdup(node->next->data);
	if (!cmd->heredoc_delimiter)
		return (1);
	return (exec_heredoc(cmd, store, last_heredoc));
}

/**
 * @brief Processes all heredoc nodes in a command
 * @param cmd Command structure
 * @param store Shell data structure
 * @return int Result of heredoc processing
 */
int	heredoc_finisher(t_cmd *cmd, t_shell *store)
{
	t_node	*tmp;
	int		result;

	tmp = cmd->redir;
	while (tmp)
	{
		if (ft_strcmp(tmp->data, "<<") == 0)
		{
			result = process_heredoc_node(cmd, store, tmp);
			if (result != EXIT_SUCCESS)
				break ;
		}
		tmp = tmp->next;
	}
	if (cmd->heredoc_write_fd > 0)
	{
		close(cmd->heredoc_write_fd);
		cmd->heredoc_write_fd = -1;
	}
	return (result);
}
