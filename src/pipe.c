/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:05:29 by seayeo            #+#    #+#             */
/*   Updated: 2024/08/29 15:52:16 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


// pls remove this

int  pipe_counter(t_node *loop)
{
	int    count;
	
	count = 0;
	while (loop)
	{
		if (ft_strcmp(loop->data, "|") == 0)
			count++;
		loop = loop->next;
	}
	return (count);
}

int	wait_for_pipes(t_shell *store, int amount)
{
	int	i;
	int	status;
	
	i = 0;
	while (i < amount)
	{
		printf("Waiting for PID: %d\n", store->pid[i]);
		fflush(stdout);
		waitpid(store->pid[i], &status, 0);
		if (WIFEXITED(status))
		{
			printf("PID %d exited with status: %d\n", store->pid[i], WEXITSTATUS(status));
			fflush(stdout);
			t_exit_status = WEXITSTATUS(status);
		}
		i++;
	}
return (EXIT_SUCCESS);
}

void	run_cmd(t_cmd *cmd, t_shell *store)
{    // Execute command
    if (check_builtin(cmd->command) == 0)
    {
		printf("Executing non-builtin command: %s\n", cmd->command->data);
        fflush(stdout);
        t_exit_status = executor(store, cmd->command, NULL);
        printf("Non-builtin command executed with exit status: %d\n", t_exit_status);
        fflush(stdout);
        exit(t_exit_status);
    }
    else
    {
 		printf("Executing builtin command: %s\n", cmd->command->data);
        fflush(stdout);
        t_exit_status = builtin_main(store, cmd->command, cmd->redir);
        printf("Builtin command executed with exit status: %d\n", t_exit_status);
        fflush(stdout);
        exit(t_exit_status);
    }
}

void open_fd(t_cmd *cmd, t_shell *store, int end[2])
{
    printf("Opening file descriptors for command: %s\n", cmd->command->data);
    fflush(stdout);

    // Handle input redirection
    if (store->input_fd != 0)
    {
        if (dup2(store->input_fd, STDIN_FILENO) < 0)
        {
            perror("dup2 failed for input");
            exit(EXIT_FAILURE);
        }
		close(store->input_fd);
    }
    // Handle output redirection
    if (cmd->next)
    {
        if (dup2(end[1], STDOUT_FILENO) < 0)
        {
            perror("dup2 failed for output");
            exit(EXIT_FAILURE);
        }
    }
	if (cmd->next)
	{
		// close(end[0]);j
		close(end[1]);
	}
    printf("File descriptors opened for command: %s\n", cmd->command->data);
    fflush(stdout);
	run_cmd(cmd, store);
	exit(EXIT_FAILURE);
}

int	ft_fork(t_shell *store, int end[2], t_cmd *cmd, int i)
{
	int nbytes;

	printf("Forking process\n");
    fflush(stdout);
	store->pid[i] = fork();
	if (store->pid[i] < 0)
		print_error("Fork failed");
	if (store->pid[i] == 0)
	{
		printf("In child process for: %s\n", store->cmd_head->command->data);
        fflush(stdout);
		open_fd(cmd, store, end);
	}
	else
	{
		printf("Forked process with PID: %d\n", store->pid[i]);
        fflush(stdout);
        // close(end[1]);
		wait(NULL);
		close(end[1]);
		store->input_fd = end[0];
	}
	printf("Forked process with PID: %d\n", store->pid[i - 1]);
    fflush(stdout);
	return (EXIT_SUCCESS);
}

int multi_executor(t_shell *store, int num_pipes)
{
	int		end[2];
	t_cmd	*temp;
	int		i;

	i = 0;
	temp = store->cmd_head;
	while (store->cmd_head)
	{
		if (store->cmd_head->next)
		{
			if(pipe(end) == -1)
				print_error("Pipe failed");
			printf("Pipe created: read end = %d, write end = %d\n", end[0], end[1]);
		}
		else
			end[1] = STDOUT_FILENO;
		printf("Forking for command: %s\n", store->cmd_head->command->data);
		fflush(stdout);
		// redir_handler(store, store->cmd_head->redir, NULL);
		ft_fork(store, end, store->cmd_head, i);
		close(end[1]);
		if (store->input_fd != STDIN_FILENO)
			close(store->input_fd);
		store->input_fd = end[0];
		printf("Input fd set to: %d\n", store->input_fd);
		// fd_in = check_fd_heredoc(store, end, store->cmd_head);
		store->cmd_head = store->cmd_head->next;
		i++;
	}
    close(store->input_fd);
	store->cmd_head = temp;
	printf("Waiting for all child processes\n");
    fflush(stdout);
	wait_for_pipes(store, i);
	return (0);
}
