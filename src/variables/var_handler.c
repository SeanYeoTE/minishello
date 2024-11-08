/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 21:40:48 by mchua             #+#    #+#             */
/*   Updated: 2024/11/08 21:40:48 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

t_var	*create_var_node(char *var, char *data)
{
	t_var	*new_var;

	new_var = (t_var *)malloc(sizeof(t_var));
	if (!new_var)
		perror("Failed to allocate memory");
	new_var->name = ft_strdup(var);
	new_var->data = ft_strdup(data);
	new_var->hidden = NULL;
	new_var->next = NULL;
	return (new_var);
}

bool	same_var(char *src, t_shell *store)
{
	t_var	*current;
	int		count;

	count = name_counter(src);
	current = store->var;
	if (!current)
		return (false);
	else
	{
		while (current)
		{
			if (ft_strcmp(src, current->hidden) == 0)
				return (true);
			else if (ft_strncmp(src, current->hidden, count) == 0
				&& ft_strcmp(src, current->hidden) != 0)
			{
				free (current->hidden);
				current->hidden = ft_strdup(src);
				return (true);
			}
			current = current->next;
		}
	}
	return (false);
}

bool	same_env(char *src, t_shell *store)
{
	t_env	*current;
	int		count;

	count = name_counter(src);
	current = store->env;
	while (current)
	{
		if (ft_strcmp(src, current->var) == 0)
			return (false);
		else if (ft_strncmp(src, current->var, count) == 0
			&& ft_strcmp(src, current->var) != 0)
		{
			free (current->var);
			current->var = ft_strdup(src);
			return (false);
		}
		current = current->next;
	}
	return (true);
}

t_var	*split_var(char *src)
{
	t_var	*new_var;
	char	*name;
	char	*value;
	char	**split_array;

	split_array = ft_split_var(src, '=');
	name = split_array[0];
	value = split_array[1];
	new_var = create_var_node(name, value);
	new_var->hidden = ft_strdup(src);
	freechararray(split_array);
	return (new_var);
}

int	var_handler(char *src, t_shell *store)
{
	t_var	*new_var;
	t_var	*current;

	new_var = NULL;
	if (!same_env(src, store) || !same_var(src, store))
		new_var = split_var(src);
	if (!new_var)
		return (EXIT_FAILURE);
	if (store->var == NULL)
		store->var = new_var;
	else
	{
		current = store->var;
		while (current)
		{
			if (current->next == NULL && new_var != NULL)
			{
				current->next = new_var;
				break ;
			}
			current = current->next;
		}
	}
	return (EXIT_SUCCESS);
}
