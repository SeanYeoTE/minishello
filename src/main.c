/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/09/24 12:42:50 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		t_exit_status;

void	free_env(t_env **env)
{
	t_env	*tmp;
	t_env	*current;

	if (NULL == env)
		return ;
	current = *env;
	while (current)
	{
		tmp = current->next;
		free(current->var);
		free(current);
		current = tmp;
	}
	*env = NULL;
}

void	init_var(t_shell *store, t_env *env_head, t_var *var_head, char **envp)
{		
	store->input_fd = STDIN_FILENO;
	store->output_fd = STDOUT_FILENO;
	store->head = NULL;
	store->tail = NULL;
	store->fd_in = STDIN_FILENO;
	store->quotes = false;

	store->cmd_head = NULL;
	store->cmd_tail = NULL;
	
	store->path = getenv("PATH");
	store->paths = ft_split(store->path, ':');
	store->envp = ft_split(store->path, ':');
	store->pid = NULL;

	store->env = env_head;
	store->var = var_head;
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	store;
	t_env	*env_head;
	
	if (argc != 1 || argv[1])
	{
		perror("run without args");
		return (0);
	}

	env_head = env_init(&store, envp);
	init_var(&store, env_head, NULL, envp);
	return (prompter(&store, env_head, NULL));
}
