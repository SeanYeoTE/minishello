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

#include "../../includes/minishell.h" 
static bool	flag_checker(char *src)
{
	int		i;
	bool	n_flag;

	i = 0;
	n_flag = false;
	if (src[i] != '-')
		return (n_flag);
	if (src[i] == '-' && src[i + 1] == '\0')
		return (n_flag);
	i++;
	while (src[i] != '\0' && src[i] == 'n')
		i++;
	if (src[i] == '\0')
		n_flag = true;
	return (n_flag);
}

int	echo_handler(t_node *current)
{
	bool	n_flag;
	bool	printed;

	n_flag = false;
	current = current->next;
	if (current && flag_checker(current->data))
	{
		if (current->next != NULL)
			current = current->next;
		n_flag = true;
		while (current != NULL && flag_checker(current->data))
			current = current->next;
	}
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
