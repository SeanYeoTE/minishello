/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/10 13:40:20 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

static int	count_quotes(const char *str, int len)
{
	int		i;
	int		quote_count;
	bool	in_single_quotes;
	bool	in_double_quotes;

	i = 0;
	quote_count = 0;
	in_single_quotes = false;
	in_double_quotes = false;
	while (i < len)
	{
		if (str[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			quote_count++;
		}
		else if (str[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			quote_count++;
		}
		i++;
	}
	return (quote_count);
}

static char	*quote_remover(const char *str, int len)
{
	char	*ret;
	bool	in_single_quotes;
	bool	in_double_quotes;
	int		i;
	int		index;

	index = 0;
	i = 0;
	ret = (char *)malloc(len - count_quotes(str, len) + 1);
	if (!ret)
		print_error("malloc failed", NULL);
	in_single_quotes = false;
	in_double_quotes = false;
	while (i < len)
	{
		if (str[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (str[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else
			ret[index++] = str[i];
		i++;
	}
	ret[index] = '\0';
	return (ret);
}

void	remove_quote(t_node *token)
{
	int		len;
	char	*new;

	while (token)
	{
		if (ft_strchr(token->data, '"') || ft_strchr(token->data, '\''))
		{
			len = ft_strlen(token->data);
			new = quote_remover(token->data, len);
			free(token->data);
			token->data = new;
		}
		token = token->next;
	}
}
