/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 20:52:37 by mchua             #+#    #+#             */
/*   Updated: 2024/11/08 20:52:37 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

int	cd_handler(t_node *current)
{
	char	*home;

	if (current->next->next != NULL)
	{
		ft_putstr_fd("cd: too many arguments", 2);
		return (EXIT_FAILURE);
	}
	if (current->next == NULL || ft_strcmp(current->next->data, "~") == 0)
	{
		home = getenv("HOME");
		if (home == NULL || chdir(home) != 0)
		{
			perror("no home");
			return (BUILTIN_FAILURE);
		}
	}
	else if (chdir(current->next->data) != 0)
	{
		perror(current->data);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
