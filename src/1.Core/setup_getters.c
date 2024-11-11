/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_getters.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 12:53:02 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 15:13:13 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Custom getenv implementation for the shell
 * @param var Environment variable name to look up
 * @param env Environment variables list
 * @return Value of the environment variable or NULL if not found
 * @details Searches through the environment list for a variable
 *          matching the provided name and returns its value
 */
char	*ft_getenv(char *var, t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strncmp(var, current->var, ft_strlen(var)) == 0)
		{
			if (current->var[ft_strlen(var)] == '=')
				return (current->var + ft_strlen(var) + 1);
		}
		current = current->next;
	}
	return (NULL);
}

/**
 * @brief Counts the number of environment variables
 * @param env Environment variables list
 * @return Number of environment variables in the list
 */
static int	count_env_entries(t_env *env)
{
	t_env	*current;
	int		count;

	count = 0;
	current = env;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/**
 * @brief Creates an array of environment variables
 * @param env Environment variables list
 * @return NULL-terminated array of environment variable strings
 * @details Converts the linked list of environment variables into
 *          an array format suitable for execve and other functions
 */
char	**ft_createarray(t_env *env)
{
	t_env	*current;
	char	**new_env;
	int		i;
	int		size;

	size = count_env_entries(env);
	new_env = (char **)malloc(sizeof(char *) * (size + 1));
	if (!new_env)
	{
		perror("Failed to allocate memory for environment");
		exit(1);
	}
	i = 0;
	current = env;
	while (current)
	{
		new_env[i++] = current->var;
		current = current->next;
	}
	new_env[i] = NULL;
	return (new_env);
}

/**
 * @brief Forms the shell prompt string
 * @param cwd Current working directory
 * @return Formatted prompt string
 * @details Creates a prompt string in the format "username:cwd$ "
 *          The returned string must be freed by the caller
 */
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
