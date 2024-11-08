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

#include "../core/minishell.h"

// static bool	is_valid_var_name(char *arg)
// {
// 	int	i;

// 	i = 0;
// 	if (!arg || !ft_isalpha(arg[i]))
// 		return (false);
// 	while (arg[i])
// 	{
// 		if (!ft_isalnum(arg[i]) && arg[i] != '_' && arg[i] != ';')
// 			return (false);
// 		i++;
// 	}
// 	return (true);
// }

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

static	void	print_unset_error(char *arg)
{
	ft_putstr_fd("unset: ", 1);
	ft_putstr_fd(arg, 1);
	ft_putstr_fd(": invalid parameter name\n", 1);
}

int	unset_handler(t_shell *store)
{
	char	*arg;

	if (!(store->cmd_head->command->next))
		return (EXIT_SUCCESS);
	arg = store->cmd_head->command->next->data;
	if (got_equal(arg))
	{
		print_unset_error(arg);
		return (BUILTIN_FAILURE);
	}
	if (!perform_unset(arg, store))
		return (EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}
