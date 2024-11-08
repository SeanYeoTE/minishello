/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 20:55:35 by mchua             #+#    #+#             */
/*   Updated: 2024/11/08 20:55:35 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

t_env	*get_env_loc(t_env *env_list, char *arg)
{
	int	count;
	int	i;

	count = -1;
	i = 0;
	while (arg[i] && arg[i] != '=')
	{
		count++;
		i++;
	}
	while (env_list)
	{
		if ((ft_strncmp(env_list->var, arg, count) == 0)
			&& (ft_strncmp(env_list->var, arg, count + 1) == 0))
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

t_var	*get_var_loc(char *arg, t_var *var_list, t_env *current_env)
{
	int		count;
	char	*var_data;

	count = 0;
	if (!var_list)
		return (NULL);
	count = ft_strlen(arg);
	while (var_list)
	{
		var_data = var_list->name;
		if (ft_strncmp(var_data, arg, count) == 0
			&& ft_strcmp(arg, current_env->var) != 0)
			return (var_list);
		var_list = var_list->next;
	}
	return (NULL);
}

t_env	*create_env_node(char *var)
{
	t_env	*new_env;

	new_env = ft_calloc(1, sizeof(t_env));
	if (!new_env)
		perror("Failed to allocate memory");
	new_env->var = ft_strdup(var);
	new_env->next = NULL;
	return (new_env);
}

t_env	*env_init(char **envp)
{
	t_env	*current;
	t_env	*head;
	t_env	*new_node;
	int		i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = create_env_node(envp[i]);
		if (head == NULL)
		{
			head = new_node;
			current = head;
		}
		else
			current = current->next;
		current->next = new_node;
		i++;
	}
	return (head);
}

int	env_handler(t_shell *store)
{	
	t_env	*current;

	current = store->env;
	while (current)
	{
		ft_putstr_fd(current->var, store->cmd_head->output_fd);
		ft_putstr_fd("\n", store->cmd_head->output_fd);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}
