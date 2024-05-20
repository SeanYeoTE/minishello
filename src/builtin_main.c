/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:40:05 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/20 21:52:23 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_main(t_shell *store)
{
	if (store->head == "echo")
		echo_handler(store);
	if (store->head == "pwd")
		pwd_handler(store);
	else if (store->head == "|")
		pipe_handler(store);
}

//to handle when there is not input. 
void echo_handler(t_shell *store)
{
	if (store->next->argvs1 == NULL)
		ft_printf("\n");
	while (store->next->argvs1)
	{
		ft_printf("%c", store->next->*argvs1);
		ft_printf(" ");
		i++;
	}
	ft_printf("\n");
}

void	pwd_handler(t_shell *store)
{
	ft_printf
}