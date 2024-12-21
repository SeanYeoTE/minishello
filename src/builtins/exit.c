/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melvin <melvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:04:19 by mchua             #+#    #+#             */
/*   Updated: 2024/12/21 12:59:34 by melvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	is_numeric(char *arg)
{
	if (*arg == '-' || *arg == '+')
		arg++;
	while (*arg)
	{
		if (ft_isdigit(*arg))
			arg++;
		else
			return (false);
	}
	return (true);
}

static void	print_exit_error(t_shell *store, int fd)
{
	if (fd == 2)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("exit: numeric argument required\n", 2);
		free_all(store);
		exit (fd);
	}
	else if (fd == 1)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("exit: too many arguments\n", 2);
		free_all(store);
		exit (fd);
	}
}

int	exit_handler(t_shell *store)
{
	char	*arg;
	int		status;

	status = 0;
	if (!(store->cmd_head->command->next))
	{
		ft_putstr_fd("exit\n", 0);
		free_all(store);
		exit(status);
	}
	arg = store->cmd_head->command->next->data;
	if (ft_strcmp(arg, "") == 0 || !is_numeric(arg))
		print_exit_error(store, 2);
	else if (store->cmd_head->command->next->next != NULL)
		print_exit_error(store, 1);
	else
	{
		status = ft_atoi(arg);
		ft_putstr_fd("exit\n", 0);
	}
	if (status < 0)
		status = status % 256;
	free_all(store);
	exit(status);
}
