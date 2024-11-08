/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 20:57:29 by mchua             #+#    #+#             */
/*   Updated: 2024/11/08 20:57:29 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

void	print_error_msg(char *arg, int ret_value)
{
	if (ret_value == 1)
	{
		ft_putstr_fd("export: '", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
	}
	if (ret_value == 2)
	{
		ft_putstr_fd("export: ", 2);
		write (2, arg, 2);
		ft_putstr_fd(": invalid option\n", 2);
	}
}

void	sort_env(char **str_array, int env_len)
{
	char	*tmp;
	int		i;
	int		swapped;

	i = 0;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (i < env_len - 1)
		{
			if ((ft_strcmp(str_array[i], str_array[i + 1])) > 0)
			{
				tmp = str_array[i];
				str_array[i] = str_array[i + 1];
				str_array[i + 1] = tmp;
				swapped = 1;
			}
			i++;
		}
		env_len--;
	}
}

void	print_export(char **str_arr, int env_len)
{
	int	i;

	i = 0;
	sort_env(str_arr, env_len);
	while (str_arr[i] != NULL)
		printf("declare -x %s\n", str_arr[i++]);
}

bool	is_in_env(t_env *env_list, char *arg)
{
	t_env	*current_env;

	current_env = get_env_loc(env_list, arg);
	if (!current_env)
		return (false);
	return (true);
}

t_env	*get_last_env(t_env *current_env)
{
	t_env	*last_env;

	last_env = current_env;
	while (last_env->next)
		last_env = last_env->next;
	return (last_env);
}
