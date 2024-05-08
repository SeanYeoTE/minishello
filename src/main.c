/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/08 13:44:20 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	execution_layer(t_shell *store)
{
	execve(store->path, store->argv, store->envp);
}

int base_shell_init(t_shell *store, char *input)
{
	int		i;

	store->argv = ft_split(input, ' ');
	store->paths = ft_split(store->path + 5, ':');
	while (store->argv[i])
		i++;
	printf("%d\n", i);
	if (i == 1)
		execution_layer(store);
	return (0);
}

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
		return (1);
	}
	store.envp = envp;
	getcwd(cwd, sizeof(cwd));
	store.path = findpath(envp);
	username = finduser(envp);
	hostname = findhost(envp);
	while (1)
	{	
		if (isatty(STDOUT_FILENO))
		{
			printf("%s@%s:~%s$ ",username, hostname, cwd);
			input = readline(" ");
			base_shell_init(&store, input);
			// printf("%s\n", input);
		}
	}
	return (0);
}
