/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 13:31:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/08/09 13:15:57 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int needs_spacing(const char *input, int i)
{
	if (!is_operator(input[i]))
		return 0;
	if (i > 0 && input[i - 1] != ' ' && !is_operator(input[i - 1]))
		return 1;
	if (input[i + 1] && input[i + 1] != ' ' && 
		!is_operator(input[i + 1]))
		return 1;
	return 0;
}

static char	*add_space_before(char *str, int i)
{
	char	*front;
	char	*back;
	char	*ret;

	front = ft_substr(str, 0, i);
	back = ft_substr(str, i, ft_strlen(str) - i);
	ret = ft_strjoin(front, " ");
	free(front);
	front = ft_strjoin(ret, back);
	free(ret);
	free(back);
	return (front);	
}

static char	*add_space_after(char *str, int i, int len)
{
	char	*front;
	char	*back;
	char	*ret;

	front = ft_substr(str, 0, i + len);
	back = ft_substr(str, i + len, ft_strlen(str) - i - len);
	ret = ft_strjoin(front, " ");
	free(front);
	front = ft_strjoin(ret, back);
	free(ret);
	free(back);
	return (front);
}

// introduces spaces between operators and strings
char	*input_spacer(char *input)
{
	int		i;
	char	*ret;
	int		double_op;

	i = 0;
	while (input[i])
	{
		double_op = is_double_operator(input, i);
		if (needs_spacing(input, i))
		{
			if (i == 0 || input[i - 1] != ' ')
			{
				ret = add_space_before(input, i);
				free(input);
				input = ret;
				i++;
			}
			if (input[i + 1 + double_op] && input[i + 1 + double_op] != ' ')
			{
				ret = add_space_after(input, i, 1 + double_op);
				free(input);
				input = ret;
				i++;
			}
			i += 1 + double_op;
		}
		else
			i++;
	}
	return (input);
}
