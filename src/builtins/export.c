/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 20:58:52 by mchua             #+#    #+#             */
/*   Updated: 2024/11/08 20:58:52 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

static int	get_env_len(t_env *envp)
{
	int		i;
	t_env	*current_env;

	i = 0;
	current_env = envp;
	if (!envp)
		return (i);
	while (current_env)
	{
		i++;
		current_env = current_env->next;
	}
	return (i);
}

static void	handle_no_arg(t_env *envp)
{
	int		env_len;
	int		i;
	char	**str_array;
	t_env	*current_env;

	env_len = get_env_len(envp);
	i = 0;
	current_env = envp;
	str_array = malloc((env_len + 1) * sizeof(char *));
	if (str_array == NULL)
		return ;
	while (current_env)
	{
		str_array[i] = current_env->var;
		i++;
		current_env = current_env->next;
	}
	str_array[i] = NULL;
	print_export(str_array, env_len);
	free(str_array);
}

	//check for alpha, '_', 2
	//if -, invalid, 2
	//if other things, invalid identifier, 1
static int	check_arg(char *arg)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[i]) && arg[i] != '_')
	{
		if (arg[i] == '-')
			return (2);
		if (ft_isdigit(arg[i]) || arg[i] == '=')
			return (1);
		i++;
	}
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

static void	set_export(t_shell *store, char *arg)
{
	t_env	*current_env;
	t_var	*current_var;

	current_env = store->env;
	current_var = NULL;
	if (store->var)
		current_var = get_var_loc(arg, store->var, current_env);
	if (is_in_env(current_env, arg))
	{
		current_env = get_env_loc(store->env, arg);
		free (current_env->var);
		if (current_var)
			current_env->var = ft_strdup(current_var->hidden);
		else if (!current_var && ft_strchr(arg, '='))
			current_env->var = ft_strdup(arg);
	}
	else
	{
		current_env = get_last_env(current_env);
		if (current_var)
			current_env->next = create_env_node(current_var->hidden);
		else
			current_env->next = create_env_node(arg);
	}
}

int	export_handler(t_shell *store)
{
	char	*arg;
	t_node	*array;
	int		ret_value;

	ret_value = 0;
	if (store->cmd_head->command->next == NULL)
	{
		handle_no_arg(store->env);
		return (0);
	}
	arg = store->cmd_head->command->next->data;
	array = store->cmd_head->command->next;
	ret_value = check_arg(arg);
	print_error_msg(arg, ret_value);
	while (array)
	{
		if (ret_value == 0)
			set_export(store, arg);
		array = array->next;
		if (array)
			arg = array->data;
	}
	return (ret_value);
}
