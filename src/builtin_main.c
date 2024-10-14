/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:40:05 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/14 21:25:57 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//builtin main
int	builtin_main(t_shell *store, t_node *current, t_node *end)
{
	int		exit_status;

	exit_status = 0;
	(void)store;	
	if (ft_strcmp(current->data, "cd") == 0)
		exit_status = cd_handler(current);
	else if (!ft_strcmp(current->data, "echo"))
		exit_status = echo_handler(current, end);
	else if (!ft_strcmp(current->data, "pwd"))
		exit_status = pwd_handler(current);
	else if (!ft_strcmp(current->data, "env"))
		exit_status = env_handler(store);
	else if (!ft_strcmp(current->data, "export"))
		exit_status = export_handler(store);
	else if (!ft_strcmp(current->data, "unset"))
		exit_status = unset_handler(store);
	else if (!ft_strcmp(current->data, "exit"))
		exit_status = exit_handler(store);
	else
		exit_status = var_handler(current->data, store);
	return (exit_status);
}