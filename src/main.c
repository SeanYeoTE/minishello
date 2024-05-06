/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/06 18:11:08 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*findpath(char *envp[])
{
	int	i;

	i = 0;
	while (envp[i])
	{
		// printf("%s\n", envp[i]);
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i]);
		i++;
	}
	return (NULL);
}

char	*finduser(char *envp[])
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "USER=", 5) == 0)
			return(ft_substr(envp[i], 5, 6));
		i++;
	}
	return (NULL);
}

char	*findhost(char *envp[])
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SESSION_MANAGER=", 16) == 0)
			return(ft_substr(envp[i], 22, 6));
		i++;
	}
	return (NULL);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*store;
	char	*path;
	char	*hostname;
	char	*username;
	char	*input;

	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	path = findpath(envp);
	username = finduser(envp);
	hostname = findhost(envp);
	if (isatty(STDOUT_FILENO))
	{
		printf("%s@%s:~%s$ ",username, hostname, cwd);
		input = readline(" ");
	}
	else
	{

	}
	// return 0;
}
