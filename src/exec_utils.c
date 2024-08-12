/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/08/12 13:47:00 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// takes in the struct and processnum int, returning
// full path for execution
char	*findprocesspath(t_shell *store, char **arr)
{
	int		i;
	char	*temp;
	char	*joined;

	i = 0;
	joined = NULL;
	while ((store->paths)[i])
	{
		temp = ft_strjoin((store->paths)[i], "/");
        joined = ft_strjoin(temp, arr[0]);
		if (access(joined, X_OK) == 0)
			break ;
		free(joined);
		joined = NULL;
		i++;
	}
	free(temp);
	return (joined);
}

// fd issues, output not redirecting properly
int	executor(t_shell *store, t_node *start, t_node *end)
{
	int		execveresult;
	char	*exepath;
	char	**temp;
	
	execveresult = 0;
	temp = argv_creator(start, end);
	while (start && start->type != 3)
		start = start->next;
	exepath = findprocesspath(store, temp);
	
	if (exepath == NULL)
	{
		perror("Path not found");
		free(temp);
		t_exit_status = 127;
		return (t_exit_status);
	}
	dup2(store->output_fd, 1);
	dup2(store->input_fd, 0);
	execveresult = execve(exepath, temp, store->envp);
	if (exepath)
		free(exepath);
	free(temp);
	printf("Command executed with exit status: %d\n", t_exit_status);
	fflush(stdout);
	exit(127);
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
    close(end[1]);

    printf("File descriptors opened for command: %s\n", cmd->command->data);
    fflush(stdout);

    // Execute command
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
	fd_in = STDIN_FILENO;
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
		ft_fork(store, end, fd_in, store->cmd_head);
		if (store->cmd_head->next)
		{
			close(end[1]);
			fd_in = end[0];
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

// intention is to create an argv array for execve
char	**argv_creator(t_node *start, t_node *end)
{
	int		i;
	t_node	*temp;
	char	**ret;
	
	i = 0;
	temp = start;
	while (temp && temp != end)
	{
		temp = temp->next;
		i++;
	}
	ret = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (start && start != end)
	{
		ret[i] = ft_strdup(start->data);
		start = start->next;
		i++;
	}
	ret[i] = '\0';
	return (ret);
}
