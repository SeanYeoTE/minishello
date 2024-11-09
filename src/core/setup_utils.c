/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:04:27 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/09 18:17:21 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_var(t_shell *store, t_env *env_head, t_var *var_head)
{
	store->input_reset = dup(STDIN_FILENO);
	store->output_reset = dup(STDOUT_FILENO);
	store->head = NULL;
	store->tail = NULL;
	store->fd_in = STDIN_FILENO;
	store->quotes = false;
	store->expanded = false;
	store->cmd_head = NULL;
	store->cmd_tail = NULL;
	store->env = env_head;
	store->var = var_head;
	store->path = ft_strdup(cgetenv("PATH", env_head));
	store->paths = ft_split(store->path, ':');
	store->envp = ccreatearray(env_head);
}

char	*cgetenv(char *var, t_env *env)
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

char	**ccreatearray(t_env *env)
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
