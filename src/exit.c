/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melvin <melvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:04:19 by mchua             #+#    #+#             */
/*   Updated: 2024/10/10 19:42:36 by melvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	one_cmd(t_node *args)
{
	if (args->next == NULL)
		return true;
	else
		return false;
}

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

int	exit_handler(t_shell *store)
{
	int	status;

	status = 0;
	if (!one_cmd(store->cmd_head->command)) // means got more than 1 variable
	{
		if (!is_numeric(store->cmd_head->command->next->data)) //non numeric
		{
			fprintf(stderr, "%s: %s: numeric argument required\n", store->cmd_head->command->data, store->cmd_head->command->next->data);
			free_all(store);
			exit (2);
		}
		else if(store->cmd_head->command->next->next != NULL) //more than 2 argument
		{
			fprintf(stderr, "%s\n%s: too many arguments\n", store->cmd_head->command->data, store->cmd_head->command->data);
			free_all(store);
			exit (BUILTIN_FAILURE);
		}
		else
			status = ft_atoi(store->cmd_head->command->next->data); //handler numeric
	}
	if (status < 0) //handle negative value
		status = status % 256;
	free_all(store);
	exit (status);
}
