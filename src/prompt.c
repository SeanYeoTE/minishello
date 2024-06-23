/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:04:27 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/23 12:51:23 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*form_prompt(char *cwd)
{
	char	*username;
	// char	*host;
	char	*temp;
	char	*ret;

	username = getenv("USER");
	// host = getenv("session_manager");
	// temp = ft_strjoin(username, "@");
	// ret = ft_strjoin(temp, host);
	ret = ft_strjoin(username, " ");
	// free(temp);
	temp = ft_strjoin(ret, ":");
	free(ret);
	ret = ft_strjoin(temp, cwd);
	free(temp);
	temp = ft_strjoin(ret, "$ ");
	
	free(ret);
	
	return (temp);
}