/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:05:29 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/14 22:47:51 by seayeo           ###   ########.fr       */
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

void    pre_interpreter(t_shell *store, t_node *temp)
{
	int	pipefd[2];
	int	pid1;
	t_node	*start;
	
	start = temp;
	if (pipe(pipefd) < 0)
		printf("Pipe failed\n");
	store->output_fd = pipefd[1];
	temp = pipe_slicer(temp);
	pid1 = fork();
	if (pid1 == 0)
	{
		puts("childex\n");
		// print_stack(&temp);
		printf("temp: %s\n", temp->data);
		call_interpreter(store, start, get_end(start->next, 0));
		puts("end of child\n");
		// pipe_back(store->head, temp);
	}		
	else
	{
		wait();
		// waitpid(pid1, NULL, 0);
		if (temp->next)
			pre_interpreter(store, temp->next);
		close(store->output_fd);
		if (store->input_fd != 0)
			close(store->input_fd);
		store->input_fd = pipefd[0];
	}
	
}

void	call_interpreter(t_shell *store, t_node *start, t_node *end)
{
	int	pid1;
	
	pid1 = fork();
	if (pid1 == 0)
		interpreter(store, start, end);
	waitpid(pid1, NULL, 0);
}

t_node	*pipe_slicer(t_node *head)
{
	while (head)
	{
		if (ft_strcmp(head->data, "|") == 0)
		{
			head->prev->next = NULL;
			return (head);
		}
		head = head->next;
	}
	return (head);
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