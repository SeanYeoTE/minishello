/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 21:54:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/07 22:03:52 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

int	redir_handler(t_cmd *cmd, t_node *loop, t_node *end)
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
	return (result);
}
