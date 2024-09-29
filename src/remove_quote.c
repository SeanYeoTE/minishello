/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/09/23 19:20:16 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*quote_remover(const char *str, int len)
{
	char	*ret;
	bool	in_single_quotes;
	bool	in_double_quotes;
	int		i;
	int		index;

	index = 0;
	i = 0;
	ret = (char *)malloc(len + 1);
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
		if (token->type == 8 || token->type == 9)
		{
			len = ft_strlen(token->data);
			new = quote_remover(token->data, len);
			free(token->data);
			token->data = new;
		}
		token = token->next;
	}
}