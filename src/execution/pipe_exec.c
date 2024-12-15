/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:26:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/27 17:47:00 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 
/**
 * @brief Updates the shell's exit status based on process termination
 *
 * @param store Shell data structure to update
 * @param res Process status from waitpid
 * @note Handles both normal exit and signal termination cases
 */
static void	update_exit_status(t_shell *store, int res)
{
	if (WIFEXITED(res))
		store->exit_status = WEXITSTATUS(res);
	else if (WIFSIGNALED(res))
		store->exit_status = WTERMSIG(res) + 128;
}

/**
 * @brief Waits for all child processes to complete
 *
 * @param store Shell data structure
 * @param cmd Command list to track processes
 * @return int 0 on success
 * @note Updates exit status for each process and restores SIGINT handler
 */
static int	wait_for_processes(t_shell *store, t_cmd *cmd)
{
	int		res;

	res = 0;
	while ((waitpid(cmd->pid, &res, 0)) != -1)
	{
		update_exit_status(store, res);
		if (cmd->next)
			cmd = cmd->next;
	}
	signal(SIGINT, ctrl_c_handler);
	return (0);
}

/**
 * @brief Executes multiple commands connected by pipes
 *
 * @param store Shell data structure containing commands
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error
 * @note Handles input/output redirection between piped commands
 */
int	multi_executor(t_shell *store)
{
	int		in_fd;
	t_cmd	*cmd;
	int		out_fd;

	in_fd = STDIN_FILENO;
	cmd = store->cmd_head;
	if (heredoc_child_loop(store) != 0)
		return (EXIT_FAILURE);
	while (cmd)
	{
		if (handle_command(store, cmd, &in_fd, &out_fd) == EXIT_FAILURE)
		{
			if (!cmd->next)
				return (EXIT_FAILURE);
		}
		cmd = cmd->next;
	}
	return (wait_for_processes(store, store->cmd_head));
}

/**
 * @brief Processes a pipe token in the command list
 *
 * @param store Shell data structure
 * @param front Pointer to front of token list
 * @param back Current pipe token
 * @param create Flag indicating if new command should be created
 * @note Creates command structure and updates token list pointers
 */
static void	process_pipe_token(t_shell *store, t_node **front,
	t_node *back, bool *create)
{
	t_node	*temp;

	temp = back->next;
	create_cmd(store, *front, back->prev, *create);
	*create = false;
	free(back->data);
	free(back);
	if (temp)
		temp->prev = NULL;
	*front = temp;
}

/**
 * @brief Handles execution of multiple commands with pipes
 *
 * @param store Shell data structure
 * @return int Result of multi_executor
 * @note Parses pipe tokens and creates command structures
 */
int	multiple_function(t_shell *store)
{
	t_node	*front;
	t_node	*back;
	bool	create;

	front = store->head;
	back = store->head;
	create = true;
	while (back->next)
	{
		if (ft_strcmp(back->data, "|") == 0)
		{
			process_pipe_token(store, &front, back, &create);
			back = front;
		}
		else
			back = back->next;
	}
	create_cmd(store, front, back, create);
	return (multi_executor(store));
}
