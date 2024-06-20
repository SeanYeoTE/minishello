/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/20 17:07:53 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// scrolling up for history works, but scrolling down after messes up the prompt
int	main(void)
{
	t_shell	store;
	char	*prompt;
	char	*input;
	char 	cwd[1024];
	
	// if (argc != 1 && argv[1] != "\0")
	// {
	// 	perror("run without args");
	// 	return (0);
	// }
	signal(SIGINT, ctrl_c_handler);
	while (1)
	{
		getcwd(cwd, sizeof(cwd));
		store.path = getenv("PATH");
		store.envp = ft_split(store.path, ':');
		store.paths = ft_split(store.path, ':');
		store.head = NULL;
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
				add_history(input);
				base_shell_init(&store, input);
			}
			free(prompt);
			free(input);
		}
		// signal(SIGINT, ctrl_c_handler);
	}
	free (input);
	return (0);
}
