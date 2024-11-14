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

#include "../../includes/minishell.h"

/**
 * @brief Creates a new variable node
 *
 * Allocates memory for a new variable node and initializes its fields with
 * the provided variable name and data.
 *
 * @param var The variable name
 * @param data The variable data/value
 * @return t_var* Pointer to the newly created variable node
 */
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

/**
 * @brief Checks if a variable already exists in the shell's variable list
 *
 * Compares the source variable name with existing variables, handling both
 * exact matches and partial matches based on the variable name length.
 *
 * @param src The source variable name to check
 * @param store Shell structure containing the variable list
 * @return bool True if variable exists, false otherwise
 */
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

/**
 * @brief Checks if a variable exists in the shell's environment
 *
 * Compares the source variable name with existing environment variables,
 * handling both exact matches and partial matches based on the variable name length.
 *
 * @param src The source variable name to check
 * @param store Shell structure containing the environment variables
 * @return bool True if variable doesn't exist, false if it exists
 */
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

/**
 * @brief Splits a variable string into name and value components
 *
 * Creates a new variable node from a string in the format "name=value".
 * The string is split at the equals sign, and the components are stored
 * in a new variable node.
 *
 * @param src The source string to split
 * @return t_var* Pointer to the newly created variable node
 */
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

/**
 * @brief Handles variable assignment in the shell
 *
 * Processes variable assignments from command arguments. Currently only
 * handles single variable assignments and returns an error for multiple
 * assignments.
 *
 * @param args Linked list of command arguments
 * @param store Shell structure containing variable and environment information
 * @return int 0 on success, 1 on error
 */
int	var_handler(t_node *args, t_shell *store)
{
	int		count;
	int		ret_value;
	t_node	*current_arg;

	count = 0;
	ret_value = 0;
	current_arg = args;
	while (current_arg)
	{
		count++;
		current_arg = current_arg->next;
	}
	if (count > 1)
	{
		ft_putstr_fd("This will not be handled in Minishell\n", 2);
		ret_value = 1;
	}
	else
		ret_value = set_var(args->data, store);
	return (ret_value);
}
