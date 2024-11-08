/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 13:31:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/08 14:09:58 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

bool	within_quote(const char *input, int index)
{
	bool	in_single_quotes;
	bool	in_double_quotes;
	int		i;

	in_single_quotes = false;
	in_double_quotes = false;
	i = 0;
	while (i < index)
	{
		if (input[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		if (input[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		i++;
	}
	if (in_single_quotes || in_double_quotes)
		return (true);
	return (false);
}
