/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 20:54:34 by mchua             #+#    #+#             */
/*   Updated: 2024/11/08 20:54:34 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

int	echo_handler(t_node *current)
{
	bool	n_flag;
	bool	printed;

	n_flag = false;
	current = current->next;
	if (current)
	{
		if (ft_strcmp(current->data, "-n") == 0)
			n_flag = true;
	}
	if (n_flag)
		current = current->next;
	while (current != NULL)
	{
		ft_putstr_fd(current->data, 1);
		if (current->next)
			ft_putchar_fd(' ', 1);
		current = current->next;
	}
	printed = false;
	if (!printed && !n_flag)
		ft_putchar_fd('\n', 1);
	return (EXIT_SUCCESS);
}
