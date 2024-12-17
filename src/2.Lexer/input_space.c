/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_space.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 13:31:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 15:11:18 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	needs_spacing(const char *input, int i)
{
	if (within_quote(input, i))
		return (0);
	if (!is_operator(input[i]))
		return (0);
	if (is_double_operator(input, i))
	{
		if (i > 0 && input[i - 1] != ' ' && !is_operator(input[i - 1]))
			return (2);
		if (input[i + 2] && input[i + 2] != ' ' && !is_operator(input[i + 2]))
			return (2);
		return (0);
	}
	if (i > 0 && input[i - 1] != ' ' && !is_operator(input[i - 1]))
		return (1);
	if (input[i + 1] && input[i + 1] != ' ' && !is_operator(input[i + 1]))
		return (1);
	return (0);
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

static char	*handle_spacing(char *input, int *i, int spacing_type)
{
	char	*ret;

	if (*i == 0 || input[*i - 1] != ' ')
	{
		ret = add_space_before(input, *i);
		free(input);
		input = ret;
		(*i)++;
	}
	if (input[*i + spacing_type] && input[*i + spacing_type] != ' ')
	{
		ret = add_space_after(input, *i, spacing_type);
		free(input);
		input = ret;
		(*i)++;
	}
	*i += spacing_type;
	return (input);
}

char	*input_spacer(char *input)
{
	int		i;
	int		spacing_type;

	i = 0;
	while (input[i])
	{
		spacing_type = needs_spacing(input, i);
		if (spacing_type)
			input = handle_spacing(input, &i, spacing_type);
		else
			i++;
	}
	return (input);
}
