/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:05:29 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/14 16:50:55 by seayeo           ###   ########.fr       */
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
	int pid2;
	t_node	*temp;
	
	i = 0;
	count = pipe_counter(store->head);
	if (count == 0)
	{
		pid1 = fork();
		if (pid1 == 0)
			interpreter(store, store->head, get_last(store->head));
		else
			waitpid(pid1, NULL, 0);
		// printf("last: %s\n", get_last(store->head)->data);	
		return ;
	}
	while (i < count)
	{
		printf("i: %d\n", i);
		if (pipe(pipefd) < 0)
			printf("Pipe failed\n");
		store->output_fd = pipefd[1];
		pid1 = fork();
		if (pid1 == 0)
		{
			puts("child1\n");
			temp = pipe_replacer(store->head, i);
			interpreter(store, get_start(store->head, i), get_end(store->head, i));
			puts("child\n");
			pipe_back(store->head, temp);
		}
		pid2 = fork();
		if (pid2 == 0 && pid1 != 0)
		{
			puts("child2\n");
			temp = pipe_replacer(store->head, i + 1);
			interpreter(store, get_start(store->head, i + 1), get_end(store->head, i + 1));
			pipe_back(store->head, temp);
		}
		else
		{
			
			close(store->output_fd);
			if (store->input_fd != 0)
				close(store->input_fd);
			store->input_fd = pipefd[0];
			
		}
		waitpid(pid1, NULL, 0);
		waitpid(pid2, NULL, 0);
		i++;
	}
}

t_node	*pipe_replacer(t_node *start, int i)
{
	while (start)
	{
		if (ft_strcmp(start->data, "|") == 0)
		{
			if (i == 0)
			{
				start->prev->next = NULL;
				return (start);
			}
			i--;
		}
		start = start->next;
	}
}

t_node *pipe_back(t_node *start, t_node *temp)
{
	while (start->next)
		start = start->next;
	start->next = temp;
}

t_node	*get_start(t_node *start, int i)
{
	if (i == 0)
		return (start);
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
	// puts("get_end\n");
	while (end->next != NULL)
	{
		// printf("end: %s\n", end->data);
		if (ft_strcmp(end->data, "|") == 0)
		{
			if (i == 0)
			{
				// printf("end: %s\n", end->data);
				return (end);
			}
			i--;
		}
		end = end->next;
	}
	// puts("exit loop\n");
	// printf("end: %s\n", end->data);
	return (end);
}