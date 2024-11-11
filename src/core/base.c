/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 12:59:05 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

/**
 * @brief Main prompt loop that manages the shell's read-eval-print cycle
 * @param store Main shell data structure
 * @param state Shell state structure
 * @return EXIT_SUCCESS after successful command execution
 * @details Coordinates the shell's main operation cycle including input, processing, and execution
 */
int	minishell_loop(t_shell *store, t_shell_state *state)
{
	char			*prompt;
	bool			should_continue;

	while (1)
	{
		should_continue = false;
		prompter_init(&prompt);
		init_var(store, state);
		if (!prompter_input(store, prompt))
		{
			free_nonessential(store);
			continue;
		}
		pre_execution(store);
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

/**
 * @brief Prepares the input string for execution by handling various preprocessing steps
 * @param store Main shell data structure
 * @return EXIT_SUCCESS after successful preprocessing
 * @details Handles input spacing, variable expansions, lexical analysis, and quote removal
 */
int	pre_execution(t_shell *store)
{
	store->input = input_spacer(store->input);
	if (ft_strchr(store->input, '$') != NULL)
	{
		store->expanded = true;
		store->input = expansions(store, store->input);
	}
	full_lexer(store->input, store, 0);
	remove_quote(store->head);
	return (EXIT_SUCCESS);
}

/**
 * @brief Parses the preprocessed input and routes to appropriate execution handlers
 * @param store Main shell data structure
 * @param should_continue Pointer to boolean indicating if the loop should continue
 * @return EXIT_SUCCESS after successful parsing and execution
 * @details Determines if command requires pipe handling and routes to appropriate handler
 */
int	parser(t_shell *store, bool *should_continue)
{
	if (store->head)
	{
		if (pipe_counter(store->head) == 0)
			single_function(store, store->head, store->tail);
		else if (pipe_counter(store->head) > 0)
			multiple_function(store);
	}
	else if (store->input[0] == '\0')
	{
		free_nonessential(store);
		*should_continue = true;
		return (EXIT_SUCCESS);
	}
	free_nonessential(store);
	*should_continue = true;
	return (EXIT_SUCCESS);
}
