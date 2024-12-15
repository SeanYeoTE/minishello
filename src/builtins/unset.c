/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 21:33:13 by mchua             #+#    #+#             */
/*   Updated: 2024/11/08 21:33:13 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	got_equal(char *arg)
{
	if (ft_strchr(arg, '=') != NULL)
		return (true);
	return (false);
}

static t_env	*get_target_loc(t_env *env_list, char *arg)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (arg[i] && arg[i] != '=')
	{
		count++;
		i++;
	}
	while (env_list)
	{
		if (ft_strncmp(env_list->var, arg, count) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

static bool	perform_unset(char *arg, t_shell *store)
{
	t_env	*previous_node;
	t_env	*next_node;
	t_env	*target_env;
	t_env	*head;

	target_env = get_target_loc(store->env, arg);
	head = store->env;
	if (!target_env)
		return (false);
	previous_node = store->env;
	while (previous_node->next != target_env && previous_node != target_env)
		previous_node = previous_node->next;
	if (target_env->next)
		next_node = target_env->next;
	else
		next_node = NULL;
	free (target_env->var);
	free (target_env);
	if (head == target_env)
		store->env = next_node;
	else
		previous_node->next = next_node;
	return (true);
}

static int	unset(t_shell *store, t_node *arg)
{
	char	*current_arg;

	if (!arg->next)
		return (EXIT_SUCCESS);
	current_arg = arg->next->data;
	if (got_equal(current_arg))
		return (EXIT_SUCCESS);
	if (!perform_unset(current_arg, store))
		return (EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}

int	unset_handler(t_shell *store, t_node *args)
{
	t_node	*current_arg;
	int		ret_value;

	ret_value = 0;
	current_arg = args;
	while (current_arg)
	{
		ret_value = unset(store, current_arg);
		current_arg = current_arg->next;
	}
	return (ret_value);
}
