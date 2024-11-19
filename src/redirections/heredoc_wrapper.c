/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_wrapper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:02:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/19 20:07:24 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 

/**
 * @brief Sets up pipes for a command's heredoc
 * 
 * @param cmd Command structure to set up pipes for
 * @return int 0 on success, 1 on error
 */
static int	setup_heredoc_pipes(t_cmd *cmd)
{
	int		pipe_fds[2];

	if (checkforheredoc(cmd) == 0)
		return (0);
	if (cmd->heredoc_fd > 0)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (cmd->heredoc_write_fd > 0)
	{
		close(cmd->heredoc_write_fd);
		cmd->heredoc_write_fd = -1;
	}

	if (pipe(pipe_fds) == -1)
		return (perror("pipe"), 1);

	cmd->heredoc_write_fd = pipe_fds[1];
	cmd->heredoc_fd = pipe_fds[0];
	return (0);
}

/**
 * @brief Check if this is the last heredoc in the command
 *
 * @param current Current node in redirection list
 * @return int 1 if last heredoc, 0 otherwise
 */
static int	is_last_heredoc(t_node *current)
{
	t_node	*tmp;

	tmp = current->next->next;  // Skip current heredoc and its delimiter
	while (tmp)
	{
		if (ft_strcmp(tmp->data, "<<") == 0)
			return (0);
		tmp = tmp->next;
	}
	return (1);
}

/**
 * @brief Processes heredoc redirections for a command
 *
 * @param cmd Command structure containing redirection information
 * @return int 0 on success, 1 on error
 * @note Searches for "<<" tokens and sets up heredoc with specified delimiter
 *       Allocates memory for delimiter which must be freed by caller
 */
int	heredoc_finisher(t_cmd *cmd, t_shell* store)
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
				break;
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

/**
 * @brief Creates a child process to handle heredoc input
 *
 * @param cmd Command structure containing heredoc information
 * @param store Shell data structure
 * @param child2 Flag indicating if this is a nested child process
 * @return int 0 on success, 1 on error
 */
int	heredoc_child(t_cmd *cmd, t_shell *store)
{
	pid_t	pid;
	int		pipe_fds[2];

	if (cmd->heredoc_fd > 0)  // Close previous pipe if it exists
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (pipe(pipe_fds) == -1)
		return (perror("pipe"), 1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return (perror("fork"), 1);
	}
	if (pid == 0)
	{
		close(pipe_fds[0]);  // Child only needs write end
		cmd->heredoc_write_fd = pipe_fds[1];
		exit_wrapper(store, heredoc_finisher(cmd, store));
	}
	close(pipe_fds[1]);  // Parent only needs read end
	cmd->heredoc_fd = pipe_fds[0];
	waitpid(pid, &store->exit_status, 0);
	signal(SIGINT, ctrl_c_handler);
	return (0);
}

int	heredoc_child_loop(t_shell *store)
{
	pid_t	pid;
	t_cmd	*cmd;
	int		result;

	// Set up all pipes before forking
	cmd = store->cmd_head;
	while (cmd)
	{
		if (setup_heredoc_pipes(cmd) != 0)
			return (1);
		cmd = cmd->next;
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		cmd = store->cmd_head;
		while (cmd)
		{
			close(cmd->heredoc_fd);  // Child doesn't need read end
			result = heredoc_finisher(cmd, store);
			if (result != EXIT_SUCCESS)
				exit_wrapper(store, result);
			cmd = cmd->next;
		}
		exit_wrapper(store, EXIT_SUCCESS);
	}
	cmd = store->cmd_head;
	while (cmd)
	{
		if (cmd->heredoc_write_fd > 0)
		{
			close(cmd->heredoc_write_fd);
			cmd->heredoc_write_fd = -1;
		}
		cmd = cmd->next;
	}
	waitpid(pid, &store->exit_status, 0);
	signal(SIGINT, ctrl_c_handler);
	
	if (WIFEXITED(store->exit_status))
		return (WEXITSTATUS(store->exit_status));
	return (1);
}
