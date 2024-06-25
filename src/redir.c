/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:56:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/25 17:56:09 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*redir_handler(t_shell *store, t_node *loop, t_node *end)
{
	t_node	*temp;

	temp = loop;
	while (loop != end)
	{
		if (ft_strcmp(loop->data, ">") == 0)
			handle_output_redirection(store, loop->next->data, temp, loop);
		else if (ft_strcmp(loop->data, ">>") == 0)
			handle_append_redirection(store, loop->next->data, temp, loop);
		else if (ft_strcmp(loop->data, "<") == 0)
			handle_input_redirection(store, loop->next->data, temp, loop);
		else if (ft_strcmp(loop->data, "<<") == 0)
			handle_heredoc_redirection(store, loop->next->data);
		loop = loop->next;
	}
	return (loop);
}

void	handle_output_redirection(t_shell *store, char *filename, t_node *start, t_node *end)
{
	int	outputfd;
	
	outputfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(outputfd, store->output_fd);
	executor(store, start, end);
	close(outputfd);
}

void	handle_append_redirection(t_shell *store, char *filename, t_node *start, t_node *end)
{
	int	outputfd;
	
	outputfd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	dup2(outputfd, store->output_fd);
	executor(store, start, end);
	close(outputfd);
}

void	handle_input_redirection(t_shell *store, char *filename, t_node *start, t_node *end)
{
	int	inputfd;
	
	inputfd = open(filename, O_RDONLY);
	dup2(inputfd, store->input_fd);
	executor(store, start, end);
	close(inputfd);
}

void	handle_heredoc_redirection(t_shell *store, char *filename)
{
	int fd4 = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	
	char	*heredoc_input;
	int		pid1;
	
	while (store)
	{
		heredoc_input = readline("heredoc> ");
		if (ft_strcmp(heredoc_input, filename) == 0)
			break ;
		write(fd4, heredoc_input, ft_strlen(heredoc_input));
		write(fd4, "\n", 1);
		free(heredoc_input);
	}
	close(fd4);
	pid1 = fork();
	if (pid1 == 0)
		pre_execution(store, ft_strjoin("cat ", filename));
	else
		waitpid(pid1, NULL, 0);
	pre_execution(store, ft_strjoin("rm ", filename));
}