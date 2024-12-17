/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/17 15:31:34 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 

/**
 * @brief Main prompt loop that manages the shell's read-eval-print cycle
 * @param store Main shell data structure
 * @param state Shell state structure
 * @return EXIT_SUCCESS after successful command execution
 * @details Coordinates the shell's main operation cycle including input,
 * processing, and execution
 */
int	minishell_loop(t_shell *store, t_shell_state *state)
{
	char			*prompt;
	bool			should_continue;

	g_sig = 0;
	while (1)
	{
		should_continue = false;
		init_var(store, state);
		prompter_init(&prompt, store);
		if (!prompter_input(store, prompt))
		{
			free_nonessential(store);
			continue ;
		}
		lexer(store);
		parser(store, &should_continue);
		update_shell_state(store, state);
		if (!should_continue)
		{
			free_all(store);
			break ;
		}
	}
	return (EXIT_SUCCESS);
}
