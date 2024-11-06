/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:04:27 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/25 18:53:38 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*form_prompt(char *cwd)
{
	char	*username;
	char	*temp;
	char	*ret;

	username = getenv("USER");
	ret = ft_strjoin(username, " ");
	temp = ft_strjoin(ret, ":");
	free(ret);
	ret = ft_strjoin(temp, cwd);
	free(temp);
	temp = ft_strjoin(ret, "$ ");
	
	free(ret);
	
	return (temp);
}