/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:56:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/09/24 13:31:02 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

#define MAX_HEREDOCS 10

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

static char	*heredoc_child_process(char *delimiter, int heredoc_level)
{
	char	*heredoc_input;
	char	*prompt;
	char	*content;

	signal(SIGINT, heredoc_sigint_handler);
	prompt = ft_strjoin("heredoc", ft_itoa(heredoc_level));
	prompt = ft_strjoin(prompt, "> ");
	content = ft_strdup("");
	while (1)
	{
		heredoc_input = readline(prompt);
		if (!heredoc_input || ft_strcmp(heredoc_input, delimiter) == 0)
		{
			free(heredoc_input);
			break;
		}
		content = ft_strjoin(content, heredoc_input);
		content = ft_strjoin(content, "\n");
		free(heredoc_input);
	}
	free(prompt);
	return (content);
}

char	*handle_heredoc_redirection(t_cmd *cmd, char *delimiter, int heredoc_level)
{
	pid_t	pid;
	int		pipefd[2];
	char	*content;

	if (pipe(pipefd) == -1)
	{
		perror("Error creating pipe for heredoc");
		return (NULL);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Error forking for heredoc");
		close(pipefd[0]);
		close(pipefd[1]);
		return (NULL);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		content = heredoc_child_process(delimiter, heredoc_level);
		write(pipefd[1], content, ft_strlen(content));
		close(pipefd[1]);
		exit(0);
	}
	else
	{
		close(pipefd[1]);
		content = ft_read_fd(pipefd[0]);
		close(pipefd[0]);
		waitpid(pid, NULL, 0);
		return (content);
	}
}

void	process_heredocs(t_cmd *cmd)
{
	t_node	*loop;
	int		heredoc_count;
	char	*heredoc_contents[MAX_HEREDOCS];

	loop = cmd->redir;
	heredoc_count = 0;
	while (loop && heredoc_count < MAX_HEREDOCS)
	{
		if (ft_strcmp(loop->data, "<<") == 0)
		{
			heredoc_contents[heredoc_count] = handle_heredoc_redirection(cmd, loop->next->data, heredoc_count);
			heredoc_count++;
		}
		loop = loop->next;
	}
	if (heredoc_count > 0)
		apply_heredocs(cmd, heredoc_contents, heredoc_count);
}

void	apply_heredocs(t_cmd *cmd, char **heredoc_contents, int heredoc_count)
{
	int		pipefd[2];
	pid_t	pid;
	int		i;

	if (pipe(pipefd) == -1)
	{
		perror("Error creating pipe for heredocs");
		return;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Error forking for heredocs");
		close(pipefd[0]);
		close(pipefd[1]);
		return;
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		for (i = 0; i < heredoc_count; i++)
		{
			write(pipefd[1], heredoc_contents[i], ft_strlen(heredoc_contents[i]));
			free(heredoc_contents[i]);
		}
		close(pipefd[1]);
		exit(0);
	}
	else
	{
		close(pipefd[1]);
		dup2(pipefd[0], cmd->input_fd);
		close(pipefd[0]);
		waitpid(pid, NULL, 0);
	}
}

char	*ft_read_fd(int fd)
{
	char	*content;
	char	buffer[1024];
	int		bytes_read;

	content = ft_strdup("");
	while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
	{
		buffer[bytes_read] = '\0';
		content = ft_strjoin(content, buffer);
	}
	return (content);
}