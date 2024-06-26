/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 18:40:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/26 14:13:38 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*get_last_cmd(t_cmd *cmd)
{
	while (cmd)
		cmd = cmd->next;
	return (cmd);
}

t_cmd	*init_cmd(t_shell *store, t_node *start, t_node *end)
{
	t_cmd	*cmd;
	// puts("init_cmd\n");
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (get_last_cmd(store->cmd_head) == NULL)
	{
		store->cmd_head = cmd;
		cmd->prev = NULL;
		cmd->next = NULL;
	}
	else
	{
		cmd->prev = get_last_cmd(store->cmd_head);
		cmd->prev->next = cmd;
	}
	store->cmd_tail = cmd;
	// puts("end of t_cmd pointer init\n");

	cmd->command = start;
	if (end->next)
		end->next = NULL;
	// print_stack(&cmd->command);
	detach_redir(cmd);
	return (cmd);
}