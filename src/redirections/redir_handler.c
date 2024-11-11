/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 21:54:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 14:28:35 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 
/**
 * @brief Handles all types of I/O redirection for a command
 *
 * @param cmd Command structure to set up redirections for
 * @param loop Current node in the token list
 * @param end End node of the token list (exclusive)
 * @return int 0 on success, non-zero on error
 * @note Processes input (<), output (>), and append (>>) redirections
 *       For each redirection token, processes the following token as the filename
 */
int	redir_handler(t_shell *store, t_cmd *cmd, t_node *loop, t_node *end)
{
	int	result;

	result = 0;
	while (loop != end && result == 0)
	{
		if (ft_strcmp(loop->data, "<") == 0)
			result = handle_input_redirection(cmd, loop->next->data);
		else if (ft_strcmp(loop->data, ">") == 0)
			result = handle_output_redirection(cmd, loop->next->data);
		else if (ft_strcmp(loop->data, ">>") == 0)
			result = handle_append_redirection(cmd, loop->next->data);
		loop = loop->next;
	}
	store->exit_status = result;
	return (result);
}
