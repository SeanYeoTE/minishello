/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/10 19:50:31 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

/**
 * @brief Finds the full path of an executable in PATH directories
 *
 * @param store Shell data structure containing PATH directories
 * @param arr Array of command arguments (arr[0] is the command name)
 * @return char* Full path to executable if found, NULL otherwise
 * @note Searches each PATH directory for an executable matching the command name
 *       Returns a newly allocated string that must be freed by the caller
 */
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

/**
 * @brief Checks if a path points to a directory
 *
 * @param path Path to check
 * @return int 1 if path is a directory, 0 if not, -1 on error
 * @note Uses stat to determine if the path is a directory
 *       Returns -1 if stat fails (e.g., path doesn't exist)
 */
int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (-1);
	return (S_ISDIR(path_stat.st_mode));
}
