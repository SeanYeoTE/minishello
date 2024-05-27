/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/27 12:51:57 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// scrolling up for history works, but scrolling down after messes up the prompt
int	main(int argc, char **argv, char **envp)
{
	t_shell	store;
	char	*path;
	char	*hostname;
	char	*username;
	char	*input;

	char 	cwd[1024];
	if (argc != 1)
	{
		perror("run without args");
		return (0);
	}
	store.envp = envp;
	username = finduser(envp);
	hostname = findhost(envp);
	while (1)
	{	
		getcwd(cwd, sizeof(cwd));
		store.path = findpath(envp);
		
		if (isatty(STDOUT_FILENO))
		{
			printf("%s@%s:~%s$ ",username, hostname, cwd);
			input = readline(" ");
			add_history(input);
			base_shell_init(&store, input);
			free(input);
		}
	}
	return (0);
}
