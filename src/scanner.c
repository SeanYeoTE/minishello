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

#include "minishell.h"

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
	bool	count;
	char	*data;
	int		type;

	store->quotes = false;
	i = 0;
	count = true;
	type = 9;
	while (str[start + i])
	{
		if (str[start + i] == '"' || str[start + i] == '\'')
		{
			if (str[start + i] == '"')
				type = 8;
			count = !count;
			if (count == true)
			{
				if (str[start + i + 1] && str[start + i + 1] == ' ')
					break;
			}
		}
		i++;
	}
	data = ft_strndup(str + start, i + 1);
	init_node(data, &store->head);
	store->tail = get_last(store->head);
	get_last(store->head)->type = type;
	return (start + i + 1);
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
		if (str[start + i] != ' ')
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
		if (str[start + i] == ' ')
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
	return (start + i);
}
