/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_state.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:34:18 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/14 12:58:02 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Handles the processing of dollar signs within quotes
 *
 * @param input Pointer to the input string being processed
 * @param i Pointer to the current position in the input string
 * @param quote_state Structure containing the current quote state
 */
void	handle_dollar_quotes(char **input, int *i, t_quote_state *quote_state)
{
	char	*new_input;

	if (quote_state->in_double_quotes)
	{
		(*i) += 2;
		return ;
	}
	else
	{
		new_input = remove_dollar_quotes(*input, *i);
		*input = new_input;
		*i = 0;
	}
}

/**
 * @brief Handles the expansion of the $? special variable for exit status
 *
 * @param store Shell structure containing the exit status
 * @param input Pointer to the input string being processed
 * @param i Pointer to the current position in the input string
 */
void	handle_exit_status(t_shell *store, char **input, int *i)
{
	char	*new_input;

	new_input = replace_exit_status(store, *input, *i);
	free(*input);
	*input = new_input;
	*i = 0;
}

/**
 * @brief Handles the expansion of environment variables
 *
 * @param input Pointer to the input string being processed
 * @param i Pointer to the current position in the input string
 * @param store Shell structure containing environment variables
 */
void	handle_variable(char **input, int *i, t_shell *store)
{
	char	*var;
	char	*value;
	char	*new_input;
	int		end;

	var = extract_var_name(*input, *i, &end);
	if (!var)
	{
		(*i)++;
		return ;
	}
	value = ft_getenv(var, store->env);
	if (!value)
		value = "";
	new_input = replace_var(*input, *i, end, value);
	free(*input);
	free(var);
	*input = new_input;
	*i = 0;
}

/**
 * @brief Initializes the quote state for variable expansion
 *
 * @param in_single_quotes Pointer to the single quote state flag
 * @param in_double_quotes Pointer to the double quote state flag
 */
void	init_expansion_state(bool *in_single_quotes, bool *in_double_quotes)
{
	*in_single_quotes = false;
	*in_double_quotes = false;
}
