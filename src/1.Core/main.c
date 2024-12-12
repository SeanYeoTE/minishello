/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 12:14:09 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 

volatile sig_atomic_t	g_sig;

/**
 * @brief Entry point of the minishell program
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @param envp Array of environment variables
 * @return Exit status of the shell program
 * @details Initializes the shell environment and starts the main command loop.
 *          Validates that no command line arguments are provided, as the shell
 *          should be run without arguments. Sets up the environment variables
 *          and launches the main shell prompt.
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell			store;
	t_env			*env_head;
	t_shell_state	state;

	if (argc != 1 || argv[1])
	{
		perror("run without args");
		return (0);
	}
	env_head = env_init(envp);
	init_shell_state(&state, env_head);
	return (minishell_loop(&store, &state));
}

void	init_shell_state(t_shell_state *state, t_env *env_head)
{
	state->env = env_head;
	state->var = NULL;
	state->exit_status = 0;
}
