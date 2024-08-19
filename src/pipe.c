/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:05:29 by seayeo            #+#    #+#             */
/*   Updated: 2024/08/15 17:23:38 by seayeo           ###   ########.fr       */
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

int	wait_for_pipes(int *pid, int amount)
{
	int	i;
	int	status;

	i = 0;
	while (i < amount)
	{
		printf("Waiting for PID: %d\n", pid[i]);
		fflush(stdout);
		waitpid(pid[i], &status, 0);
		if (WIFEXITED(status)) {
            printf("PID %d exited with status: %d\n", pid[i], WEXITSTATUS(status));
			fflush(stdout);
		}
		i++;
	}
	waitpid(pid[i], &status, 0);
	if (WIFEXITED(status))
		t_exit_status = WEXITSTATUS(status);
	return (EXIT_SUCCESS);
}

void	run_cmd(t_cmd *cmd, t_shell *store);

void open_fd(t_cmd *cmd, t_shell *store, int end[2], int fd_in)
{
    printf("Opening file descriptors for command: %s\n", cmd->command->data);
    fflush(stdout);

    // Handle input redirection
    if (cmd->prev)
    {
        if (dup2(fd_in, STDIN_FILENO) < 0)
        {
            perror("dup2 failed for input");
            exit(EXIT_FAILURE);
        }
        close(fd_in);
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
    // Close unused pipe ends
    if (cmd->next)
        close(end[0]);
    printf("File descriptors opened for command: %s\n", cmd->command->data);
    fflush(stdout);
	run_cmd(cmd, store);
}

void	run_cmd(t_cmd *cmd, t_shell *store)
{    // Execute command
    if (check_builtin(cmd->command) == 0)
    {
        printf("Executing builtin command: %s\n", cmd->command->data);
        fflush(stdout);
        t_exit_status = builtin_main(store, cmd->command, cmd->redir);
        printf("Builtin command executed with exit status: %d\n", t_exit_status);
        fflush(stdout);
        exit(t_exit_status);
    }
    else
    {
        printf("Executing non-builtin command: %s\n", cmd->command->data);
        fflush(stdout);
        t_exit_status = executor(store, cmd->command, NULL);
        printf("Non-builtin command executed with exit status: %d\n", t_exit_status);
        fflush(stdout);
        exit(t_exit_status);
    }
}

int	ft_fork(t_shell *store, int end[2], int fd_in, t_cmd *cmd)
{
	static int	i = 0;

	printf("Forking process\n");
    fflush(stdout);
	store->pid[i] = fork();
	if (store->pid[i] < 0)
		print_error("Fork failed");
	if (store->pid[i] == 0)
	{
		printf("In child process for: %s\n", store->cmd_head->command->data);
        fflush(stdout);
		open_fd(cmd, store, end, fd_in);
	}
	else
		store->fd_in = end[0];
	i++;
	printf("Forked process with PID: %d\n", store->pid[i - 1]);
    fflush(stdout);
	return (EXIT_SUCCESS);
}

int multi_executor(t_shell *store, int num_pipes)
{
	int		end[2];
	int		fd_in;
	t_cmd	*temp;
	
	temp = store->cmd_head;
	while (store->cmd_head)
	{
		if (store->cmd_head->next)
		{
			printf("Creating pipe\n");
            fflush(stdout);
			if (pipe(end) < 0)
				print_error("Pipe failed");
		}
		else
		{
			end[1] = STDOUT_FILENO;
		}
		printf("Forking for command: %s\n", store->cmd_head->command->data);
		fflush(stdout);
		redir_handler(store, store->cmd_head->redir, NULL);
		printf(store->fd_in == STDIN_FILENO ? "STDIN_FILENO\n" : "NOT STDIN_FILENO\n");
		ft_fork(store, end, store->fd_in, store->cmd_head);
		if (store->cmd_head->next)
		{
			close(end[1]);
			store->fd_in = end[0];
		}
		// fd_in = check_fd_heredoc(store, end, store->cmd_head);
		store->cmd_head = store->cmd_head->next;
	}
	store->cmd_head = temp;
	printf("Waiting for all child processes\n");
    fflush(stdout);
	wait_for_pipes(store->pid, store->pipes);
	return (0);
}
