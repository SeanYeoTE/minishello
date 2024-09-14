/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:56:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/09/14 12:21:14 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*redir_handler(t_cmd *cmd, t_node *loop, t_node *end)
{
	t_node	*temp;
	
	temp = loop;
	while (loop != end)
	{
		if (ft_strcmp(loop->data, ">") == 0)
			handle_output_redirection(cmd, loop->next->data, temp, loop);
		else if (ft_strcmp(loop->data, ">>") == 0)
			handle_append_redirection(cmd, loop->next->data, temp, loop);
		else if (ft_strcmp(loop->data, "<") == 0)
			handle_input_redirection(cmd, loop->next->data, temp, loop);
		// else if (ft_strcmp(loop->data, "<<") == 0)
		// 	handle_heredoc_redirection(cmd, loop->next->data);
		loop = loop->next;
	}
	return (loop);
}

// change redirs to be stored in cmd struct, executor can check for input output fds,
// to account for redirects and pipes occuring together

void	handle_output_redirection(t_cmd *cmd, char *filename, t_node *start, t_node *end)
{
	int	outputfd;
	
	outputfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	dup2(outputfd, cmd->output_fd);
}

void	handle_append_redirection(t_cmd *cmd, char *filename, t_node *start, t_node *end)
{
	int	outputfd;
	
	outputfd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	dup2(outputfd, cmd->output_fd);
}

void	handle_input_redirection(t_cmd *cmd, char *filename, t_node *start, t_node *end)
{
	int	inputfd;
	
	inputfd = open(filename, O_RDONLY);
	dup2(inputfd, cmd->input_fd);
}

// void	handle_heredoc_redirection(t_shell *store, char *filename)
// {
// 	int fd4 = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	
// 	char	*heredoc_input;
// 	int		pid1;
	
// 	while (store)
// 	{
// 		heredoc_input = readline("heredoc> ");
// 		if (ft_strcmp(heredoc_input, filename) == 0)
// 			break ;
// 		write(fd4, heredoc_input, ft_strlen(heredoc_input));
// 		write(fd4, "\n", 1);
// 		free(heredoc_input);
// 	}
// 	close(fd4);
// 	pid1 = fork();
// 	if (pid1 == 0)
// 		pre_execution(store, ft_strjoin("cat ", filename));
// 	else
// 		waitpid(pid1, NULL, 0);
// 	pre_execution(store, ft_strjoin("rm ", filename));
// }