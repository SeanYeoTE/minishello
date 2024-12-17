/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:02:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 12:15:42 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Counts the number of quotes in a string
 *
 * This function counts both single and double quotes while respecting quote
 * nesting rules. A quote is only counted if it's not inside another type
 * of quote.
 * @param str The string to count quotes in
 * @param len Length of the string
 * @return int The total number of quotes found
 */
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

/**
 * @brief Removes quotes from a string while preserving nested content
 *
 * This function creates a new string with all quotes removed, while maintaining
 * the content between quotes. It handles both single and double quotes, and
 * respects quote nesting rules.
 *
 * @param str The string to remove quotes from
 * @param len Length of the string
 * @return char* A new string with quotes removed
 */
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

/**
 * @brief Handles quote removal for the first token
 *
 * This function specifically handles quote removal for the first token
 * (when count is 0).
 * It processes tokens of type 8 or 9 that have a length greater than 2.
 *
 * @param token The token to process
 * @return char* The processed token data, or NULL if no processing was needed
 */
static char	*handle_first_token(t_node *token)
{
	char	*new;

	if (token->data && (token->type == 8 || token->type == 9))
	{
		if (ft_strlen(token->data) > 2)
		{
			new = quote_remover(token->data, ft_strlen(token->data));
			return (new);
		}
	}
	return (NULL);
}

/**
 * @brief Removes quotes from all tokens in a linked list
 *
 * This function processes each token in the linked list, removing both single
 * and double quotes from their data while preserving the content between quotes.
 * It only processes tokens that contain quotes.
 *
 * @param token The head of the token linked list
 */
void	remove_quote(t_node *token)
{
	char	*new;
	int		count;

	count = 0;
	while (token)
	{
		if (count == 0)
		{
			new = handle_first_token(token);
			if (new)
			{
				free(token->data);
				token->data = new;
			}
		}
		else if (ft_strchr(token->data, '"') || ft_strchr(token->data, '\''))
		{
			new = quote_remover(token->data, ft_strlen(token->data));
			free(token->data);
			token->data = new;
		}
		count++;
		token = token->next;
	}
}

// if quotes are closed and there are other characters in the string, return 1
// else return 0
int	should_remove_quote(t_node *head)
{
	t_node	*current;

	if (!head)
		return (0);
	current = head;
	while (current)
	{
		if (current->data && (current->type == 8 || current->type == 9))
		{
			if (ft_strlen(current->data) > 2)
				return (1);
		}
		current = current->next;
	}
	return (0);
}
