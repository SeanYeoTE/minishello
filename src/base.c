/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/14 16:45:31 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// introduces spaces between operators and strings
char	*input_spacer(char *input)
{
	int 	i;
	char	*front;
	char	*back;

	i = 0;
	while (input[i])
	{
		if (input[i] == '<' || input[i] == '>')
		{
			if (input[i - 1] != ' ')
			{
				front = ft_substr(input, 0, i);
				back = ft_substr(input, i, ft_strlen(input) - i);
				input = ft_strjoin(ft_strjoin(front, " "), back);
				i = 0;
			}
			if (input[i + 1] != ' ')
			{
				front = ft_substr(input, 0, i + 1);
				back = ft_substr(input, i + 1, ft_strlen(input) - i - 1);
				input = ft_strjoin(ft_strjoin(front, " "), back);
				i = 0;
			}
		}
	}
}

void	base_shell_init(t_shell *store, char *input)
{
	int		i;
	int		pid1;

	store->head = NULL;
	ft_sscan(input, store, 0);
	// print_stack(&store->head);
	// pid1 = fork();
	// if (pid1 == 0)
	pre_interpreter(store);
	// else
	free_nonessential(store);
	// waitpid(pid1, NULL, 0);
}

void	interpreter(t_shell *store, t_node *loop, t_node *end)
{
	end = end->next;
	print_stack_se(loop, end);
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
		else
			loop = builtin_main(store, loop, end);
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
