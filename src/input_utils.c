/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 13:31:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/25 06:06:24 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	within_quote(const char *input, int index)
{
	bool	in_single_quotes;
	bool	in_double_quotes;
	
	in_single_quotes = false;
	in_double_quotes = false;
	int i = 0;
	while (i < index)
	{
		if (input[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		if (input[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		i++;
	}
	if (in_single_quotes || in_double_quotes)
		return true;
	return false;
}

static int needs_spacing(const char *input, int i)
{
	if (within_quote(input, i))
		return 0;
	if (!is_operator(input[i]))
		return 0;
	if (is_double_operator(input, i))
	{
		if (i > 0 && input[i - 1] != ' ' && !is_operator(input[i - 1]))
			return 2;
		if (input[i + 2] && input[i + 2] != ' ' && !is_operator(input[i + 2]))
			return 2;
		return 0;
	}
	if (i > 0 && input[i - 1] != ' ' && !is_operator(input[i - 1]))
		return 1;
	if (input[i + 1] && input[i + 1] != ' ' && !is_operator(input[i + 1]))
		return 1;
	return 0;
}

static char	*add_space_before(char *str, int i)
{
	char	*front;
	char	*back;
	char	*ret;
	char	*result;

	front = ft_substr(str, 0, i);
	back = ft_substr(str, i, ft_strlen(str) - i);
	ret = ft_strjoin(front, " ");
	free(front);
	result = ft_strjoin(ret, back);
	free(ret);
	free(back);
	return (result);	
}

static char	*add_space_after(char *str, int i, int len)
{
	char	*front;
	char	*back;
	char	*ret;
	char	*result;

	front = ft_substr(str, 0, i + len);
	back = ft_substr(str, i + len, ft_strlen(str) - i - len);
	ret = ft_strjoin(front, " ");
	free(front);
	result = ft_strjoin(ret, back);
	free(ret);
	free(back);
	return (result);
}

char	*input_spacer(char *input)
{
	int		i;
	char	*ret;
	int		spacing_type;

	i = 0;
	while (input[i])
	{
		spacing_type = needs_spacing(input, i);
		if (spacing_type)
		{
			if (i == 0 || input[i - 1] != ' ')
			{
				ret = add_space_before(input, i);
				free(input);
				input = ret;
				i++;
			}
			if (input[i + spacing_type] && input[i + spacing_type] != ' ')
			{
				ret = add_space_after(input, i, spacing_type);
				free(input);
				input = ret;
				i++;
			}
			i += spacing_type;
		}
		else
			i++;
	}
	return (input);
}
