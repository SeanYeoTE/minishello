/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_state.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:34:18 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/10 19:30:08 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

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

void	handle_exit_status(t_shell *store, char **input, int *i)
{
	char	*new_input;

	new_input = replace_exit_status(store, *input, *i);
	free(*input);
	*input = new_input;
	*i = 0;
}

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
	value = cgetenv(var, store->env);
	if (!value)
		value = "";
	new_input = replace_var(*input, *i, end, value);
	free(*input);
	free(var);
	*input = new_input;
	*i = 0;
}

void	init_expansion_state(bool *in_single_quotes, bool *in_double_quotes)
{
	*in_single_quotes = false;
	*in_double_quotes = false;
}
