/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/25 18:30:07 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		t_exit_status;

// t_exit_status = 0;

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

// init var
void	init_var(t_shell *store)
{		
	store->input_fd = dup(0);
	store->output_fd = dup(1);
	store->head = NULL;
	store->tail = NULL;

	store->first = NULL;
	store->last = NULL;
	
	store->path = getenv("PATH");
	store->envp = ft_split(store->path, ':');
	store->paths = ft_split(store->path, ':');
}

// scrolling up for history works, but scrolling down after messes up the prompt
int	main(int argc, char **argv, char **envp)
{
	t_shell	store;
	char	*prompt;
	char	*input;
	
	
	if (argc != 1 || argv[1])
	{
		perror("run without args");
		return (0);
	}
	signal(SIGINT, ctrl_c_handler);
	// env_init(&store, envp);
	prompter(&store);
}
