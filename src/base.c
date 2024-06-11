/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/10 17:08:19 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	base_shell_init(t_shell *store, char *input)
{
	int		i;
	int		pid1;

	store->head = NULL;
	ft_sscan(input, store, 0);
	pid1 = fork();
	if (pid1 == 0)
		interpreter(store);
	else
		free_nonessential(store);
	waitpid(pid1, NULL, 0);
}

void	interpreter(t_shell *store)
{
	t_node	*loop;
	
	loop = store->head;
	
	while (loop)
	{
		// check if any pipes;

		// check if any redirection;
		if (redir_checker(store, loop) == 1)
			loop = redir_handler(store, loop);
		// check if any $ to expand;

		// normal executions;
		if (check_builtin(store, loop) == 0)
			loop = executor(store, loop);
		// exec builtins if any;
		else
			loop = builtin_main(store, loop);
	}
}

int	check_builtin(t_shell *store, t_node *loop)
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

int redir_checker(t_shell *store, t_node *loop)
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
