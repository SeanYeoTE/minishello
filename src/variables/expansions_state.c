/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_state.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:34:18 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/09 05:15:41 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

void	handle_dollar_quotes(char **input, int *i)
{
	*input = remove_dollar_quotes(*input, *i);
	*i = 0;
}

void	handle_exit_status(char **input, int *i)
{
	char	*new_input;

	new_input = replace_exit_status(*input, *i);
	free(*input);
	*input = new_input;
	*i = 0;
}

void	handle_variable(char **input, int *i, t_shell *store)
{
	char	*var;
	char	*temp;
	char	*new_input;
	int		end;
	char	*value;

	var = extract_var_name(*input, *i, &end);
	if (var == NULL)
	{
		(*i)++;
		return ;
	}
	temp = cgetenv(var, store->env);
	if (temp)
		value = temp;
	else
		value = "";
	new_input = replace_var(*input, *i, end, value);
	free(*input);
	*input = new_input;
	free(var);
	*i = 0;
}

void	init_expansion_state(bool *in_single_quotes, bool *in_double_quotes)
{
	*in_single_quotes = false;
	*in_double_quotes = false;
}
