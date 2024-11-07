/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/07 22:22:33 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

char	*findprocesspath(t_shell *store, char **arr)
{
	int		i;
	char	*temp;
	char	*joined;

	i = 0;
	while ((store->paths)[i])
	{
		temp = ft_strjoin((store->paths)[i], "/");
		if (!temp)
			return (NULL);
		joined = ft_strjoin(temp, arr[0]);
		free(temp);
		if (!joined)
			return (NULL);
		if (access(joined, X_OK) == 0)
			return (joined);
		free(joined);
		i++;
	}
	return (NULL);
}

int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (-1);
	return (S_ISDIR(path_stat.st_mode));
}
