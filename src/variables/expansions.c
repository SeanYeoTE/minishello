/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:34:18 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/09 04:09:51 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

static void	handle_expansion_char(char **input, int *i, t_shell *store,
	bool *in_single_quotes, bool *in_double_quotes)
{
	if ((*input)[*i] == '$' && (*input)[*i + 1] == '"' && !*in_single_quotes)
		handle_dollar_quotes(input, i);
	else if ((*input)[*i] == '$' && (*input)[*i + 1] == '?' && 
		should_expand(*in_single_quotes))
		handle_exit_status(input, i);
	else if ((*input)[*i] == '$' && should_expand(*in_single_quotes))
	{
		handle_variable(input, i, store);
		init_expansion_state(in_single_quotes, in_double_quotes);	
	}
	else
		(*i)++;
}

char	*expansions(t_shell *store, char *input)
{
	int		i;
	bool	in_single_quotes;
	bool	in_double_quotes;

	i = 0;
	init_expansion_state(&in_single_quotes, &in_double_quotes);
	while (input[i])
	{
		update_quote_state(input[i], &in_single_quotes, &in_double_quotes);
		if (in_single_quotes)
		{
			i++;
			continue;
		}
		handle_expansion_char(&input, &i, store, &in_single_quotes, &in_double_quotes);
	}
	return (input);
}
