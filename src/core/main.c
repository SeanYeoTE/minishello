/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/10 19:38:33 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	g_exit_status;

int	main(int argc, char **argv, char **envp)
{
	t_shell	store;
	t_env	*env_head;

	if (argc != 1 || argv[1])
	{
		perror("run without args");
		return (0);
	}
	env_head = env_init(envp);
	return (prompter(&store, env_head, NULL, 0));
}
