/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:34:18 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/09 17:41:06 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

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
		handle_exit_status(input, i);
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
