/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:34:18 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 12:15:03 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Handles different types of expansion characters in the input
 *
 * This function processes special characters like $", $?, and $ followed by
 * variable names. It delegates the handling to appropriate functions based on
 * the type of expansion needed and the current quote state.
 *
 * @param input Pointer to the input string being processed
 * @param i Pointer to the current position in the input string
 * @param store Shell structure containing environment and state information
 * @param quote_state Structure containing the current quote state
 */
static void	handle_expansion_char(char **input, int *i, t_shell *store,
	t_quote_state *quote_state)
{
	if ((*input)[*i] == '$' && (*input)[*i + 1] == '\"' &&
		!quote_state->in_single_quotes)
	{
		handle_dollar_quotes(input, i, quote_state);
		init_expansion_state(&quote_state->in_single_quotes,
			&quote_state->in_double_quotes);
	}
	else if ((*input)[*i] == '$' && (*input)[*i + 1] == '?' &&
		should_expand(quote_state->in_single_quotes))
		handle_exit_status(store, input, i);
	else if ((*input)[*i] == '$' &&
		should_expand(quote_state->in_single_quotes))
	{
		handle_variable(input, i, store);
		init_expansion_state(&quote_state->in_single_quotes,
			&quote_state->in_double_quotes);
	}
	else
		(*i)++;
}

/**
 * @brief Main function for handling variable expansions in the input string
 *
 * This function processes the entire input string, handling variable expansions
 * while respecting quote states. It expands environment variables, the exit
 * status variable ($?), and handles special quote cases.
 *
 * @param store Shell structure containing environment and state information
 * @param input The input string to process
 * @return char* The processed string with all expansions performed
 */
char	*expansions(t_shell *store, char *input)
{
	int				i;
	t_quote_state	quote_state;

	i = 0;
	init_expansion_state(&quote_state.in_single_quotes,
		&quote_state.in_double_quotes);
	while (input[i])
	{
		update_quote_state(input[i], &quote_state.in_single_quotes,
			&quote_state.in_double_quotes);
		if (quote_state.in_single_quotes)
		{
			i++;
			continue ;
		}
		handle_expansion_char(&input, &i, store, &quote_state);
	}
	return (input);
}
