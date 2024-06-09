/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:04:27 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/03 15:15:21 by seayeo           ###   ########.fr       */
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
	char	**temp;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "SESSION_MANAGER=", 16) == 0)
		{
			temp = ft_split(envp[i], ':');
			return(ft_strdup(temp[0] + 22));
		}
		i++;
	}
	return (NULL);
}

char	*form_prompt(char **envp, char *cwd)
{
	char	*username;
	char	*host;
	char	*temp;
	char	*ret;

	username = finduser(envp);
	host = findhost(envp);
	temp = ft_strjoin(username, "@");
	ret = ft_strjoin(temp, host);
	ret = ft_strjoin(ret, ":");
	ret = ft_strjoin(ret, cwd);
	ret = ft_strjoin(ret, "$ ");
	return (ret);
}