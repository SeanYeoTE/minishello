/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 21:54:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 12:17:02 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 

/**
 * @brief Sets up heredoc input if available
 *
 * @param cmd Command structure containing heredoc information
 * @return int 0 on success, non-zero on error
 * @note Uses heredoc_fd if available, setting it as the input source
 */
// static int	setup_heredoc_input(t_cmd *cmd)
// {
// 	if (cmd->heredoc_fd > 0)
// 	{
// 		if (cmd->input_fd != STDIN_FILENO)
// 			close(cmd->input_fd);
// 		cmd->input_fd = cmd->heredoc_fd;
// 		cmd->input_changed = true;
// 		cmd->heredoc_fd = -1;  // Prevent double-close
// 		return (0);
// 	}
// 	return (0);
// }

/**
 * @brief Handles all types of I/O redirection for a command
 *
 * @param cmd Command structure to set up redirections for
 * @param loop Current node in the token list
 * @param end End node of the token list (exclusive)
 * @return int 0 on success, non-zero on error
 * @note Processes input (<), output (>), and append (>>) redirections
 *       For each redirection token, processes the following token as the
 *       filename
 *       Prioritizes heredoc input over file input when both exist
 */
int	redir_handler(t_shell *store, t_cmd *cmd, t_node *loop, t_node *end)
{
	int	result;

	result = 0;
	while (loop != end && result == 0)
	{
		if (ft_strcmp(loop->data, "<") == 0 && loop->next)
			result = handle_input_redirection(cmd, loop->next->data);
		else if (ft_strcmp(loop->data, ">") == 0 && loop->next)
			result = handle_output_redirection(cmd, loop->next->data);
		else if (ft_strcmp(loop->data, ">>") == 0 && loop->next)
			result = handle_append_redirection(cmd, loop->next->data);
		loop = loop->next;
	}
	store->exit_status = result;
	return (result);
}
