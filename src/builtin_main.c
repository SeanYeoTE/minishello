/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:40:05 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/21 16:02:48 by mchua            ###   ########.fr       */
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
	store = store->next;
	if (store->argvs1[1] == n)
	{
		store = store->next;
		while (store)
		{
			ft_printf("%c", store->next->*argvs1);
			ft_printf(" ");
			store = store->next;
		}
	}
	else
	{
		if (store == NULL)
			ft_printf("'\n");
		store = store->next;
		while (store)
		{
			ft_printf("%c", store->next->argvs1);
			ft_printf(" ");
			store = store->next;
		}
		ft_printf("\n");
	}
}

void	pwd_handler(t_shell *store)
{
	char	*cwd;
	size_t	cwd_buf;

	cwd = malloc (cwd_buf * sizeof(char));
	if (cwd == NULL)
		//handle error;
	if (getcwd(cwd, size) != NULL)
		ft_printf("%s\n", cwd);
	else
	{
		//handle error
		free (cwd);
	}
}

void	pipe_handler(t_shell *store)
{

}