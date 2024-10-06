/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:40:05 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/02 16:17:14 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//builtin main
int	builtin_main(t_shell *store, t_node *current, t_node *end)
{
	int		exit_status;
	(void)store;
	
	if (ft_strcmp(current->data, "cd") == 0)
		exit_status = cd_handler(current);
	else if (!ft_strcmp(current->data, "echo"))
		exit_status = echo_handler(current, end);
	else if (!ft_strcmp(current->data, "pwd"))
		exit_status = pwd_handler(current);
	else if (!ft_strcmp(current->data, "env"))
		env_handler(store);
	else if (!ft_strcmp(current->data, "export"))
		exit_status = export_handler(store);
	else if (!ft_strcmp(current->data, "unset"))
		exit_status = unset_handler(store);
	else if (!ft_strcmp(current->data, "exit"))
		exit_status = exit_handler(store);
	else
		exit_status = var_handler(current->data, store);
	//exit(exit_status);
	return (exit_status);
}