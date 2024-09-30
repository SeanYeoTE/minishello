/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 14:56:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/09/30 15:15:03 by seayeo           ###   ########.fr       */
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
