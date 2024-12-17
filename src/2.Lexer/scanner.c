/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 14:48:52 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 15:12:13 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	return (start + i);
}
