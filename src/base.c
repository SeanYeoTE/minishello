/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/20 13:53:44 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// introduces spaces between operators and strings
char	*input_spacer(char *input)
{
	int 	i;
	char	*front;
	char	*back;
	int		detected;

	i = 0;
	detected = 0;
	while (input[i])
	{
		if (input[i] == '<' || input[i] == '>')
			detected = 1;
		else
		{
			if (detected == 1)
			{
				front = ft_substr(input, 0, i);
				back = ft_substr(input, i, ft_strlen(input) - i);
				input = ft_strjoin(ft_strjoin(front, " "), back);
				free(front);
				free(back);
				i++;
			}
			detected = 0;
		}
		i++;
	}
	return (input);
}

void	base_shell_init(t_shell *store, char *input)
{
	store->head = NULL;
	input = input_spacer(input);
	if (ft_strchr(input, '$') != NULL)
		input = expansions(input);
	ft_sscan(input, store, 0);
	// print_stack(&store->head);
	if (store->head)
	{
		if (pipe_counter(store->head) == 0)
			call_interpreter(store, store->head, store->tail);
		else if (pipe_counter(store->head) > 0)
			pre_interpreter(store, store->head);
		free_nonessential(store);
	}
}

void	interpreter(t_shell *store, t_node *loop, t_node *end)
{
	end = end->next;
	// print_stack_se(loop, end);
	while (loop != end)
	{
		// check if any pipes;

		// check if any redirection;
		if (redir_checker(loop) == 1)
			loop = redir_handler(store, loop, end);
		// check if any $ to expand;

		// normal executions;
		if (check_builtin(loop) == 0)
			loop = executor(store, loop, end);
		// exec builtins if any;
		// else
		// 	loop = builtin_main(store, loop, end);
	}
}

int	check_builtin(t_node *loop)
{
	if (ft_strcmp(loop->data, "echo") == 0)
		return (1);
	if (ft_strcmp(loop->data, "cd") == 0)
		return (1);
	if (ft_strcmp(loop->data, "pwd") == 0)
		return (1);	
	if (ft_strcmp(loop->data, "export") == 0)
		return (1);
	else
		return (0);
}

int redir_checker(t_node *loop)
{
	while (loop)
	{
		if (ft_strcmp(loop->data, ">") == 0)
			return (1);
		if (ft_strcmp(loop->data, ">>") == 0)
			return (1);
		if (ft_strcmp(loop->data, "<") == 0)
			return (1);
		if (ft_strcmp(loop->data, "<<") == 0)
			return (1);
		loop = loop->next;
	}
	return (0);
}
