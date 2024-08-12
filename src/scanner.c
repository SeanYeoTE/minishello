/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 14:48:52 by seayeo            #+#    #+#             */
/*   Updated: 2024/08/12 17:29:06 by seayeo           ###   ########.fr       */
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
	int		offset;

	store->quotes = false;
	i = 0;
	count = true;
	offset = 0;
	while (str[start + i])
	{
		// printf("str[start + i]: %c\n", str[start + i]);
		// fflush(stdout);
		if (str[start + i] == '"' || str[start + i] == '\'')
		{
			if (count == true)
				offset = i;
			count = !count;
			if (count == true)
				break ;
		}
		i++;
	}
	if ((str[start] == '"' || str[start] == '\''))
	{
		printf("hi\n");
		data = ft_strndup(str + start + 1, i - 1);
	}
	else
		data = ft_strndup(str + start, i);
	init_node(data, &store->head);
	store->tail = get_last(store->head);
	get_last(store->head)->type = 2;
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
