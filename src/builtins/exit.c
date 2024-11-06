/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:04:19 by mchua             #+#    #+#             */
/*   Updated: 2024/11/06 13:08:57 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

// static bool	one_cmd(t_node *args)
// {
// 	if (args->next == NULL)
// 		return true;
// 	else
// 		return false;
// }

static bool	is_numeric(char *arg)
{
	if (*arg == '-' || *arg == '+')
		arg++;
	while (*arg)
	{
		if (ft_isdigit(*arg))
			arg++;
		else
			return false;
	}
	return true;
}

static void	print_error_msg(t_shell *store, int fd)
{
	if (fd == 2)
	{
		ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("exit: numeric argument required\n", 2);
		free_all(store);
		exit (fd);
	}
	else
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
		free_all(store);
		exit(status);
	}
	arg = store->cmd_head->command->next->data;
	if (ft_strcmp(arg, "") == 0 || !is_numeric(arg))
		print_error_msg(store, 2);
	else if (store->cmd_head->command->next->next != NULL)
		print_error_msg(store, 1);
	else
		status = ft_atoi(arg);
	if (status < 0)
		status = status % 256;
	free_all(store);
	exit(status);
}
