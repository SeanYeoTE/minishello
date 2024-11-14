/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_wrapper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:02:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/14 20:31:47 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 
/**
 * @brief Processes heredoc redirections for a command
 *
 * @param cmd Command structure containing redirection information
 * @return int 0 on success, 1 on error
 * @note Searches for "<<" tokens and sets up heredoc with specified delimiter
 *       Allocates memory for delimiter which must be freed by caller
 */
int	heredoc_finisher(t_cmd *cmd, t_shell* store, int child)
{
	t_node	*tmp;
	int		result;

	result = 0;
	tmp = cmd->redir;
	while (tmp)
	{
		if (ft_strcmp(tmp->data, "<<") == 0)
		{
			cmd->heredoc_delimiter = ft_strdup(tmp->next->data);
			if (cmd->heredoc_delimiter == NULL)
				return (1);
			if (child)
				result = heredoc_single_builtin(cmd, store);
			else
				result = heredoc_single_external(cmd, store);
			if (result != EXIT_SUCCESS)
			{
				if (!child)
				{
					free(cmd->heredoc_delimiter);
					cmd->heredoc_delimiter = NULL;
				}
				break;
			}
			if (!child)
			{
				free(cmd->heredoc_delimiter);
				cmd->heredoc_delimiter = NULL;
			}
		}
		tmp = tmp->next;
	}
	if (child)
		free_all(store);
	return (result);
}

/**
 * @brief Processes all heredoc inputs for commands in pipeline
 *
 * @param store Shell data structure containing command list
 * @return int EXIT_SUCCESS if all heredocs processed, EXIT_FAILURE on error
 * @note Must be called before executing pipeline to set up all heredoc inputs
 */
int	handle_all_heredocs(t_shell *store)
{
	t_cmd	*cmd;
	int		result;

	result = 0;
	cmd = store->cmd_head;
	while (cmd)
	{
		result = heredoc_finisher(cmd, store, 0);
		if (result != 0)
			return (EXIT_FAILURE);
		cmd = cmd->next;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Creates a child process to handle heredoc input
 *
 * @param cmd Command structure containing heredoc information
 * @param store Shell data structure
 * @param child2 Flag indicating if this is a nested child process
 * @return int 0 on success, 1 on error
 */
int	heredoc_child(t_cmd *cmd, t_shell *store, int child2)
{
	pid_t	pid;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		exit(heredoc_finisher(cmd, store, 1));
	}
	waitpid(pid, &store->exit_status, 0);
	if (child2 == 0)
		signal(SIGINT, ctrl_c_handler);
	return (0);
}
