/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:40:05 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/27 17:47:50 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_main(t_shell *store, t_node *current)
{
	int		exit_status;

	exit_status = 0;
	(void)store;
	if (ft_strcmp(current->data, "cd") == 0)
		exit_status = cd_handler(current);
	else if (!ft_strcmp(current->data, "echo"))
		exit_status = echo_handler(current);
	else if (!ft_strcmp(current->data, "pwd"))
		exit_status = pwd_handler();
	else if (!ft_strcmp(current->data, "env"))
		exit_status = env_handler(store);
	else if (!ft_strcmp(current->data, "export"))
		exit_status = export_handler(store, current);
	else if (!ft_strcmp(current->data, "unset"))
		exit_status = unset_handler(store, current);
	else if (!ft_strcmp(current->data, "exit"))
		exit_status = exit_handler(store);
	else
		exit_status = var_handler(current, store);
	return (exit_status);
}
