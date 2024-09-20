/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:05:29 by seayeo            #+#    #+#             */
/*   Updated: 2024/09/20 14:10:33 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** pipe_counter: Counts the number of pipe symbols in the command chain.
** @loop: Pointer to the first node of the command chain.
** 
** This function traverses the linked list of command nodes and counts
** the number of pipe symbols ("|"). It's used to determine how many
** pipe operations are needed for the given command sequence.
**
** Return: The number of pipe symbols found.
*/
int	pipe_counter(t_node *loop)
{
	int	count;
	
	count = 0;
	while (loop)
	{
		if (ft_strcmp(loop->data, "|") == 0)
			count++;
		loop = loop->next;
	}
	return (count);
}

/*
** wait_for_command: Waits for a child process to complete and updates the exit status.
** @pid: Process ID of the child process to wait for.
**
** This function uses waitpid to wait for the specified child process to finish.
** It then updates the global exit status (t_exit_status) based on how the
** child process terminated (normal exit or signal termination).
**
** Return: Always returns EXIT_SUCCESS.
*/
int	wait_for_command(pid_t pid)
{
	int	status;
	
	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		t_exit_status = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status))
	{
		t_exit_status = WTERMSIG(status) + 128;
	}
	return (EXIT_SUCCESS);
}

/*
** run_cmd: Executes a single command, either a builtin or an external program.
** @cmd: Pointer to the command structure.
** @store: Pointer to the shell's global data structure.
**
** This function checks if the command is a builtin. If it is, it executes
** the builtin directly. If not, it calls the executor function to run
** an external program. After execution, it exits the child process with
** the appropriate exit status.
*/
void	run_cmd(t_cmd *cmd, t_shell *store)
{
	if (check_builtin(cmd->command) == 0)
	{
		t_exit_status = executor(store, cmd->command, NULL);
		exit(t_exit_status);
	}
	else
	{
		t_exit_status = builtin_main(store, cmd->command, cmd->redir);
		exit(t_exit_status);
	}
}

/*
** setup_pipes: Sets up input and output file descriptors for a command.
** @in_fd: Input file descriptor.
** @out_fd: Output file descriptor.
** @cmd: Pointer to the command structure.
**
** This function sets up the input and output file descriptors for a command
** in a pipeline. It handles redirections and ensures that the command
** reads from the correct input and writes to the correct output.
*/
void	setup_pipes(int in_fd, int out_fd, t_cmd *cmd)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (cmd->redir && cmd->output_fd != STDOUT_FILENO)
	{
		dup2(cmd->output_fd, STDOUT_FILENO);
		close(cmd->output_fd);
	}
	else if (out_fd != STDOUT_FILENO)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
}

/*
** execute_command: Forks a child process to execute a single command.
** @store: Pointer to the shell's global data structure.
** @cmd: Pointer to the command structure.
** @in_fd: Input file descriptor.
** @out_fd: Output file descriptor.
**
** This function creates a child process using fork(). In the child process,
** it sets up the pipes, handles redirections, and runs the command.
** The parent process returns the PID of the child.
**
** Return: PID of the child process, or EXIT_FAILURE if fork fails.
*/
int	execute_command(t_shell *store, t_cmd *cmd, int in_fd, int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("Fork failed");
		return EXIT_FAILURE;
	}
	if (pid == 0)
	{
		setup_pipes(in_fd, out_fd, cmd);
		redir_handler(cmd, cmd->redir, NULL);
		run_cmd(cmd, store);
	}
	return pid;
}

/*
** setup_pipe: Creates a new pipe.
** @pipe_fds: Array to store the read and write file descriptors of the pipe.
**
** This function creates a new pipe using the pipe() system call.
** It's used to set up communication between commands in a pipeline.
**
** Return: EXIT_SUCCESS if pipe creation is successful, EXIT_FAILURE otherwise.
*/
int	setup_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
	{
		perror("Pipe failed");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/*
** handle_pipe_fds: Manages file descriptors after a command in a pipeline.
** @in_fd: Pointer to the input file descriptor.
** @pipe_fds: Array containing the read and write file descriptors of the pipe.
** @is_last_cmd: Flag indicating if this is the last command in the pipeline.
**
** This function closes unnecessary file descriptors and sets up the input
** for the next command in the pipeline if there is one.
*/
void	handle_pipe_fds(int *in_fd, int pipe_fds[2], int is_last_cmd)
{
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (!is_last_cmd)
	{
		close(pipe_fds[1]);
		*in_fd = pipe_fds[0];
	}
}

/*
** execute_and_wait: Executes a command and waits for it if it's the last in a pipeline.
** @store: Pointer to the shell's global data structure.
** @cmd: Pointer to the command structure.
** @in_fd: Input file descriptor.
** @out_fd: Output file descriptor.
** @is_last_cmd: Flag indicating if this is the last command in the pipeline.
**
** This function executes a single command and, if it's the last command
** in a pipeline, waits for it to complete and updates the exit status.
**
** Return: EXIT_SUCCESS if execution is successful, EXIT_FAILURE otherwise.
*/
int	execute_and_wait(t_shell *store, t_cmd *cmd, int in_fd, int out_fd, int is_last_cmd)
{
	pid_t	last_pid;

	last_pid = execute_command(store, cmd, in_fd, out_fd);
	if (last_pid == EXIT_FAILURE)
		return EXIT_FAILURE;
	if (is_last_cmd)
		wait_for_command(last_pid);
	else
		waitpid(last_pid, NULL, 0);
	return EXIT_SUCCESS;
}

/*
** handle_command: Sets up and executes a single command in a pipeline.
** @store: Pointer to the shell's global data structure.
** @cmd: Pointer to the command structure.
** @in_fd: Pointer to the input file descriptor.
** @out_fd: Pointer to the output file descriptor.
**
** This function prepares the execution environment for a single command
** in a pipeline. It sets up pipes if necessary, executes the command,
** and manages file descriptors afterwards.
**
** Return: EXIT_SUCCESS if handling is successful, EXIT_FAILURE otherwise.
*/
int	handle_command(t_shell *store, t_cmd *cmd, int *in_fd, int *out_fd)
{
	int		pipe_fds[2];
	int		is_last_cmd;

	is_last_cmd = (cmd->next == NULL);
	if (!is_last_cmd)
	{
		if (setup_pipe(pipe_fds) == EXIT_FAILURE)
			return EXIT_FAILURE;
		*out_fd = pipe_fds[1];
	}
	else
	{
		*out_fd = STDOUT_FILENO;
	}
	if (execute_and_wait(store, cmd, *in_fd, *out_fd, is_last_cmd) == EXIT_FAILURE)
		return EXIT_FAILURE;
	handle_pipe_fds(in_fd, pipe_fds, is_last_cmd);
	return EXIT_SUCCESS;
}

/*
** multi_executor: Executes a pipeline of commands.
** @store: Pointer to the shell's global data structure.
** @num_pipes: Number of pipes in the command sequence (unused in this implementation).
**
** This function is the main driver for executing a pipeline of commands.
** It iterates through the linked list of commands, setting up pipes between
** them and executing each command in sequence.
**
** Return: EXIT_SUCCESS if all commands execute successfully, EXIT_FAILURE otherwise.
*/
int	multi_executor(t_shell *store, int num_pipes)
{
	int		in_fd;
	t_cmd	*cmd;
	int		out_fd;

	in_fd = STDIN_FILENO;
	cmd = store->cmd_head;
	while (cmd)
	{
		if (handle_command(store, cmd, &in_fd, &out_fd) == EXIT_FAILURE)
			return EXIT_FAILURE;
		cmd = cmd->next;
	}
	return EXIT_SUCCESS;
}