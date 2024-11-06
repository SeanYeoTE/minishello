/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 14:48:52 by seayeo            #+#    #+#             */
/*   Updated: 2024/08/19 12:50:13 by seayeo           ###   ########.fr       */

/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

int	scanner_comment(char *str, int start, t_shell *store)
{
	int		i;
	char	*data;

	i = 0;
	while (str[start + i])
	{
		if (str[start + i] == '\n')
			return (start + i);
		i++;
	}
	data = ft_strndup(str + start, i + 1);
	init_node(data, &store->head);
	store->tail = get_last(store->head);
	get_last(store->head)->type = 1;
	return (start + i);
}



int	scanner_quote(char *str, int start, t_shell *store)
{
	int		i;
	char	*data;
	int		type;
	char	quote_char;
	int		matched;
	int 	y;

	y = 0;
	while (str[start + y])
	{
		if (str[start + y] == '"' || str[start + y] == '\'')
			break ;
		y++;
	}
	quote_char = str[start + y];
	matched = 0;
	store->quotes = false;
	i = 0;
	type = (quote_char == '"') ? 8 : 9;
	while (str[start + i])
	{
		if (str[start + i] == quote_char)
			matched++;
		if (matched % 2 == 0)
		{
			if (str[start + i + 1] == '"' || str[start + i + 1] == '\'')
				quote_char = str[start + i + 1];
			else if (!str[start + i + 1] || is_space(str[start + i + 1]))
				break;
		}
		i++;
	}
	i++;
	data = ft_strndup(str + start, i);
	init_node(data, &store->head);
	store->tail = get_last(store->head);
	get_last(store->head)->type = type;

	// printf("quote_char: .%c.\n", str[start + i - 1]);
	// printf("curr char: .%c.\n", str[start + i]);
	// printf("next char: .%c.\n", str[start + i + 1]);
	
	return (start + i);
}

int	scanner_operator(char *str, int start, t_shell *store)
{
	int		i;
	char	*data;

	i = 0;
	while (str[start + i])
	{
		if (str[start + i] == ' ')
			break ;
		i++;
	}
	data = ft_strndup(str + start, i);
	init_node(data, &store->head);
	store->tail = get_last(store->head);
	get_last(store->head)->type = 3;
	return (start + i);
}

int	scanner_space(char *str, int start)
{
	int		i;

	i = 0;
	while (str[start + i])
	{
		if (is_space(str[start + i]) == false)
			break ;
		i++;
	}
	return (start + i);
}

int	scanner_word(char *str, int start, t_shell *store)
{
	int		i;
	char	*data;
	i = 0;
	while (str[start + i])
	{
		if (is_space(str[start + i]))
			break ;
		if (str[start + i] == '\'' || str[start + i] == '"')
		{
			store->quotes = true;
			return (start);
		}
		i++;
	}
	data = ft_strndup(str + start, i);
	init_node(data, &store->head);
	store->tail = get_last(store->head);
	get_last(store->head)->type = 5;

	// printf("prev char: .%c.\n", str[start + i - 1]);
	// printf("curr char: .%c.\n", str[start + i]);
	// printf("next char: .%c.\n", str[start + i + 1]);
	return (start + i);
}