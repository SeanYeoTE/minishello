/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:05:29 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/11 18:17:50 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void    pre_interpreter(t_shell *store)
{
	int	count;
	int	pipefd[2];
	int	i;
	int	pid1;
	
	i = 0;
	count = pipe_counter(store->head);
	while (i < count)
	{
		if (pipe(pipefd) < 0)
			printf("Pipe failed\n");
		store->output_fd = pipefd[1];
		pid1 = fork();
		if (pid1 == 0)
			interpreter(store, get_start(store->head, i), get_end(store->head, i));
		else
		{
			waitpid(pid1, NULL, 0);
			close(store->output_fd);
			if (store->input_fd != 0)
				close(store->input_fd);
			store->input_fd = pipefd[0];
			i++;
		}
	}
}

t_node	*get_start(t_node *start, int i)
{
	while (start)
	{
		if (ft_strcmp(start->data, "|") == 0)
		{
			if (i == 0)
				return (start->next);
			i--;
		}
		start = start->next;
	}
}

t_node	*get_end(t_node *end, int i)
{
	while (end)
	{
		if (ft_strcmp(end->data, "|") == 0)
		{
			if (i == 0)
				return (end->prev);
			i--;
		}
		end = end->next;
	}
	return (end->prev);
}