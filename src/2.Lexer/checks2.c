/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <seayeo@42.sg>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 06:54:32 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/18 15:53:04 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// also for starting with pipe operator
int	check_invalid_pipe(char *input)
{
	int	i;

	i = 0;
	while (input[i] == ' ')
		i++;
	if (input[i] == '|')
		return (1);
	return (0);
}

// 34 is double quote, 39 is single quote
// returns 1 if all quotes are closed, 0 if not
int	check_quotes(char *line)
{
	int	i;
	int	sml;
	int	dbl;

	sml = 0;
	dbl = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == 34 && sml != 1)
			dbl = !dbl;
		if (line[i] == 39 && dbl != 1)
			sml = !sml;
		i++;
	}
	return (!(sml || dbl));
}

// wrapper for check_quotes and check_invalid_pipe
int	check_error(t_shell *store, char *input)
{
	if (!check_quotes(input))
	{
		store->exit_status = 2;
		return (1);
	}
	// if (check_invalid_pipe(input))
	// {
	// 	store->exit_status = 2;
	// 	return (1);
	// }
	return (0);
}
