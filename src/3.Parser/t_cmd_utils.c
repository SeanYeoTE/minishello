/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_cmd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 18:40:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 12:14:41 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 

/**
 * @brief Initializes a new command structure
 * @param store Main shell data structure
 * @param start First node of the command sequence
 * @param end Last node of the command sequence
 * @param create Boolean flag indicating if this is the first command
 * @return Pointer to newly initialized command structure, NULL if allocation
 * fails
 * @details Allocates memory for a new command structure, sets up redirection,
 * links, file descriptors and command nodes
 */
t_cmd	*init_cmd(t_shell *store, t_node *start, t_node *end, bool create)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
	{
		print_error(strerror(errno), "Failed to allocate memory for cmd");
		return (NULL);
	}
	cmd->redir = NULL;
	setup_cmd_links(store, cmd, create);
	cmd->command = start;
	cmd->input_changed = false;
	set_parent(cmd->command, cmd);
	if (end->next)
		end->next = NULL;
	detach_redir(cmd);
	init_cmd_fds(cmd);
	return (cmd);
}

/**
 * @brief Sets up command structure links in the command list
 * @param store Main shell data structure
 * @param cmd Command structure to setup links for
 * @param create Boolean flag indicating if this is the first command
 * @details If create is true, sets cmd as head of command list
 * Otherwise, appends cmd to end of existing command list
 */
void	setup_cmd_links(t_shell *store, t_cmd *cmd, bool create)
{
	t_cmd	*last;

	if (create)
	{
		cmd->prev = NULL;
		cmd->next = NULL;
		store->cmd_head = cmd;
	}
	else
	{
		last = get_last_cmd(store->cmd_head);
		cmd->prev = last;
		cmd->next = NULL;
		last->next = cmd;
	}
}

/**
 * @brief Separates redirection nodes from command nodes
 * @param new Command structure to process
 * @details Identifies redirection operators and their associated files in the
 * command sequence, removes them from the command list and adds them to the
 * redirection list
 */
void	detach_redir(t_cmd *new)
{
	t_node	*temp;
	t_node	*file;

	temp = new->command;
	while (temp)
	{
		if (redir_checker(temp) == 1)
		{
			file = temp->next;
			remove_nodes(&new->command, temp, file);
			add_to_redir(&new->redir, temp, file);
			temp = new->command;
		}
		else
			temp = temp->next;
	}
}

/**
 * @brief Initializes file descriptors for a command
 * @param cmd Command structure to initialize file descriptors for
 * @details Sets default values for input, output and heredoc file descriptors
 * Input defaults to STDIN, output to STDOUT, heredoc to -1
 */
void	init_cmd_fds(t_cmd *cmd)
{
	cmd->input_fd = STDIN_FILENO;
	cmd->output_fd = STDOUT_FILENO;
	cmd->heredoc_fd = -2;
	cmd->heredoc_delimiter = NULL;
}
