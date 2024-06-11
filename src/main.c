/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/09 17:15:50 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// scrolling up for history works, but scrolling down after messes up the prompt
int	main(int argc, char **argv, char **envp)
{
	t_shell	store;
	char	*prompt;
	char	*input;
	char 	cwd[1024];
	
	if (argc != 1)
	{
		perror("run without args");
		return (0);
	}
	store.envp = envp;
	while (1)
	{	
		getcwd(cwd, sizeof(cwd));
		store.path = getenv("PATH");
		store.paths = ft_split(store.path, ':');
		store.head = NULL;
		if (isatty(STDOUT_FILENO))
		{
			prompt = form_prompt(envp, cwd);
			input = readline(prompt);
			if (input)
			{
				if (ft_strcmp(input, "exit") == 0)
				{
					free(input);
					free(prompt);
					free_nonessential(&store);
					return (0);
				}
				add_history(input);
				base_shell_init(&store, input);
			}
			free(prompt);
			free(input);
		}
	}
	return (0);
}
