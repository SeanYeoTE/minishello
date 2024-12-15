/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/12 17:39:40 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 

// static void	handle_shlvl(t_shell *store, int flag)
// {
// 	t_env	*current_env;

// 	current_env = store->env;
// 	if (current_env == NULL)
// 		return ;
// 	if (flag == 1)
// 	{
// 		while (current_env)
// 		{
// 			if (ft_strcmp(current_env->var, "SHLVL=") == 0)
// 			{
// 				current_env++;
// 				return ;
// 			}
// 			current_env = current_env->next;
// 		}
// 	}
// 	return ;
// }

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
		prompter_init(&prompt);
		init_var(store, state);
		if (!prompter_input(store, prompt))
		{
			free_nonessential(store);
			continue ;
		}
		lexer(store);
		parser(store, &should_continue);
		// if (ft_strcmp(store->cmd_head->command->data, "./minishell"))
		// 	handle_shlvl(store, 1);
		update_shell_state(store, state);
		if (!should_continue)
		{
			free_all(store);
			break ;
		}
	}
	return (EXIT_SUCCESS);
}
