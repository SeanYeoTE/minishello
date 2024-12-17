/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_quote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 14:48:52 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 15:12:07 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	find_quote_start(char *str, int start)
{
	int	y;

	y = 0;
	while (str[start + y])
	{
		if (str[start + y] == '"' || str[start + y] == '\'')
			break ;
		y++;
	}
	return (y);
}

static int	process_quotes(char *str, int start, char quote_char, int *matched)
{
	int	i;

	i = 0;
	while (str[start + i])
	{
		if (str[start + i] == quote_char)
			(*matched)++;
		if (*matched % 2 == 0)
		{
			if (str[start + i + 1] == '"' || str[start + i + 1] == '\'')
				quote_char = str[start + i + 1];
			else if (!str[start + i + 1] || is_space(str[start + i + 1]))
				break ;
		}
		i++;
	}
	return (i);
}

static int	get_quote_type(char quote_char)
{
	if (quote_char == '"')
		return (8);
	return (9);
}

int	scanner_quote(char *str, int start, t_shell *store)
{
	int		i;
	char	*data;
	char	quote_char;
	int		matched;
	int		quote_start;

	quote_start = find_quote_start(str, start);
	quote_char = str[start + quote_start];
	matched = 0;
	store->quotes = false;
	i = process_quotes(str, start, quote_char, &matched);
	i++;
	data = ft_strndup(str + start, i);
	init_node(data, &store->head);
	store->tail = get_last(store->head);
	get_last(store->head)->type = get_quote_type(quote_char);
	return (start + i);
}
