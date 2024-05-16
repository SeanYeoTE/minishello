/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:40:05 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/10 14:01:54 by seayeo           ###   ########.fr       */
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

void echo_handler(t_shell *store)
{
	
}