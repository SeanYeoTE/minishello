/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:14:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/09/30 17:46:57 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define MAX_HEREDOCS 10

static void heredoc_sigint_handler(int sig)
{
    (void)sig;
    write(STDERR_FILENO, "\n", 1);
    exit(130);
}

static char *create_prompt(int heredoc_level)
{
    char *prompt;
    char *level_str;
    char *temp;

    level_str = ft_itoa(heredoc_level);
    temp = ft_strjoin("heredoc", level_str);
    prompt = ft_strjoin(temp, "> ");
    free(level_str);
    free(temp);
    return prompt;
}

static char *heredoc_child_process(char *delimiter, int heredoc_level)
{
    char *heredoc_input;
    char *prompt;
    char *content;
    size_t content_size;
    size_t content_len;

    signal(SIGINT, heredoc_sigint_handler);
    prompt = create_prompt(heredoc_level);
    content_size = 1024;
    content = malloc(content_size);
    content[0] = '\0';
    content_len = 0;

    while (1)
    {
        heredoc_input = readline(prompt);
        if (!heredoc_input || strcmp(heredoc_input, delimiter) == 0)
        {
            free(heredoc_input);
            break;
        }
        size_t input_len = strlen(heredoc_input) + 1; // +1 for '\n'
        if (content_len + input_len >= content_size)
        {
            content_size *= 2;
            content = realloc(content, content_size);
        }
        strcat(content, heredoc_input);
        strcat(content, "\n");
        content_len += input_len;
        free(heredoc_input);
    }
    free(prompt);
    return content;
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