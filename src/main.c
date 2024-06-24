/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/24 12:04:38 by seayeo           ###   ########.fr       */
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
	char 	cwd[1024];
	
	if (argc != 1 || argv[1])
	{
		perror("run without args");
		return (0);
	}
	signal(SIGINT, ctrl_c_handler);
	env_init(&store, envp);
	while (1)
	{
		getcwd(cwd, sizeof(cwd));
		init_var(&store);
		if (isatty(STDOUT_FILENO))
		{
			prompt = form_prompt(cwd);
			input = readline(prompt);
			if (input == NULL)
				break ;
			else if (input)
			{
				if (ft_strcmp(input, "exit") == 0)
					break ;
				else if (ft_strcmp(input, "env") == 0)
					env_handler(&store);
				add_history(input);
				base_shell_init(&store, input);
			}
			free(prompt);
			free(input);
		}
	}
	free (input);
	free_env(&store.env);
	return (0);
}
