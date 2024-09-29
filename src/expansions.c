/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 15:34:18 by seayeo            #+#    #+#             */
/*   Updated: 2024/09/29 16:59:59 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_var_name(const char *input, int start, int *end)
{
	int	j;

	j = start + 1;
	while ((ft_isalnum(input[j]) || input[j] == '_') && input[j] != '\0')
		j++;
	*end = j;
	return (ft_strndup(input + start + 1, j - start - 1));
}

static bool	single_quotes(const char *input, int index)
{
	bool	in_single_quotes;
	
	in_single_quotes = false;
	int i = 0;
	while (i < index)
	{
		if (input[i] == '\'')
			in_single_quotes = !in_single_quotes;
		i++;
	}
	return in_single_quotes;
}

// Helper function to replace variable with its value
static char	*replace_var(char *input, int start, int end, const char *value)
{
	char	*front;
	char	*back;
	size_t	result_len;
	char	*result;

	result_len = 0;
	front = ft_strndup(input, start);
	back = ft_strdup(input + end);
	result_len = ft_strlen(value) + 1;
	if (front)
		result_len += ft_strlen(front);
	if (back)
		result_len += ft_strlen(back);
	result = (char *)malloc(result_len);
	result[0] = '\0';
	if (result)
	{
		if (front != NULL)
			ft_strlcpy(result, front, ft_strlen(front) + 1);
		ft_strlcat(result, value, result_len);
		if (back != NULL)
			ft_strlcat(result, back, result_len);
	}
	free(front);
	free(back);
	return (result);
}

// Helper function to replace exit status with its value
static char	*replace_exit_status(char *input, int start)
{
	char	*front;
	char	*back;
	size_t	result_len;
	char	*result;

	result_len = 0;
	front = ft_strndup(input, start);
	back = ft_strdup(input + start + 2);
	result_len = ft_strlen(ft_itoa(t_exit_status)) + 1;
	if (front)
		result_len += ft_strlen(front);
	if (back)
		result_len += ft_strlen(back);
	result = (char *)malloc(result_len);
	result[0] = '\0';
	if (result)
	{
		if (front != NULL)
			ft_strlcpy(result, front, ft_strlen(front) + 1);
		ft_strlcat(result, ft_itoa(t_exit_status), result_len);
		if (back != NULL)
			ft_strlcat(result, back, result_len);
	}
	free(front);
	free(back);
	return (result);
}

// Main function to expand variables
char	*expansions(char *input)
{
	int		i;
	int		end;
	char	*var;
	char	*temp;
	char	*new_input;

	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] == '?')
		{
			new_input = replace_exit_status(input, i);
			free(input);
			input = new_input;
			i = 0;
		}
		else if (input[i] == '$' && !single_quotes(input, i))
		{
			var = extract_var_name(input, i, &end);
			temp = getenv(var);
			if (temp == NULL)
				temp = "";
			new_input = replace_var(input, i, end, temp);
			free(input);
			input = new_input;
			free(var);
			i = 0;
		}
		i++;
	}
	return (input);
}
