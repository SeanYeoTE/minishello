/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:56:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/09/23 17:10:08 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

t_node	*redir_handler(t_cmd *cmd, t_node *loop, t_node *end)
{
	t_node	*temp;
	
	temp = loop;
	while (loop != end)
	{
		if (ft_strcmp(loop->data, ">") == 0)
			handle_output_redirection(cmd, loop->next->data);
		else if (ft_strcmp(loop->data, ">>") == 0)
			handle_append_redirection(cmd, loop->next->data);
		else if (ft_strcmp(loop->data, "<") == 0)
			handle_input_redirection(cmd, loop->next->data);
		else if (ft_strcmp(loop->data, "<<") == 0)
			handle_heredoc_redirection(cmd, loop->next->data);
		loop = loop->next;
	}
	return (loop);
}

void	handle_output_redirection(t_cmd *cmd, char *filename)
{
	int	outputfd;
	
	outputfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outputfd == -1)
	{
		perror("Error opening output file");
		return;
	}
	dup2(outputfd, cmd->output_fd);
	close(outputfd);
}

void	handle_append_redirection(t_cmd *cmd, char *filename)
{
	int	outputfd;
	
	outputfd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (outputfd == -1)
	{
		perror("Error opening output file for append");
		return;
	}
	dup2(outputfd, cmd->output_fd);
	close(outputfd);
}

void	handle_input_redirection(t_cmd *cmd, char *filename)
{
	int	inputfd;
	
	inputfd = open(filename, O_RDONLY);
	if (inputfd == -1)
	{
		perror("Error opening input file");
		return;
	}
	dup2(inputfd, cmd->input_fd);
	close(inputfd);
}

static void heredoc_sigint_handler(int sig)
{
    (void)sig;
    write(STDERR_FILENO, "\n", 1);
    exit(130);
}

static void	heredoc_child_process(int write_fd, char *delimiter)
{
	char	*heredoc_input;

	signal(SIGINT, heredoc_sigint_handler);
	while (1)
	{
		heredoc_input = readline("heredoc> ");
		if (!heredoc_input || ft_strcmp(heredoc_input, delimiter) == 0)
		{
			free(heredoc_input);
			break;
		}
		write(write_fd, heredoc_input, ft_strlen(heredoc_input));
		write(write_fd, "\n", 1);
		free(heredoc_input);
	}
	close(write_fd);
	exit(0);
}

static int	setup_heredoc_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("Error creating pipe for heredoc");
		return (0);
	}
	return (1);
}

static int	handle_heredoc_parent(t_cmd *cmd, int read_fd, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(read_fd);
		return (0);
	}
	dup2(read_fd, cmd->input_fd);
	close(read_fd);
	return (1);
}

void	handle_heredoc_redirection(t_cmd *cmd, char *delimiter)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (!setup_heredoc_pipe(pipe_fd))
		return;
	pid = fork();
	if (pid == -1)
	{
		perror("Error forking for heredoc");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return;
	}
	if (pid == 0)
	{
		close(pipe_fd[0]);
		heredoc_child_process(pipe_fd[1], delimiter);
	}
	else
	{
		close(pipe_fd[1]);
		if (!handle_heredoc_parent(cmd, pipe_fd[0], pid))
			return;
	}
}