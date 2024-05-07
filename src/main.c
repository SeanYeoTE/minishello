/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/07 16:32:12 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int base_shell_init(t_shell *store, char **envp)
{
	
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*store;
	char	*path;
	char	*hostname;
	char	*username;
	char	*input;

	char 	cwd[1024];
	if (argc != 1)
	{
		perror("run without args");
		return (1);
	}
	store->envp = envp;
	getcwd(cwd, sizeof(cwd));
	path = findpath(envp);
	username = finduser(envp);
	hostname = findhost(envp);
	while (1)
	{	
		if (isatty(STDOUT_FILENO))
		{
			printf("%s@%s:~%s$ ",username, hostname, cwd);
			input = readline(" ");
			printf("%s\n", input);
		}
	}
	free(input);
	free(path);
	free(username);
	free(hostname);
	return (0);
}
