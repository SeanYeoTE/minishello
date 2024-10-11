/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 15:34:18 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/11 12:50:53 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_var_name(const char *input, int start, int *end)
{
	int	j;

	j = start + 1;
	if (!ft_isalpha(input[j]) && input[j] != '_')
	{
		*end = j;
		return (NULL);
	}
	while ((ft_isalnum(input[j]) || input[j] == '_') && input[j] != '\0')
		j++;
	*end = j;
	return (ft_strndup(input + start + 1, j - start - 1));
}

static void	update_quote_state(char c, bool *in_single_quotes, bool *in_double_quotes)
{
	if (c == '\'' && !*in_double_quotes)
		*in_single_quotes = !*in_single_quotes;
	else if (c == '"' && !*in_single_quotes)
		*in_double_quotes = !*in_double_quotes;
}

static bool	should_expand(bool in_single_quotes, bool in_double_quotes)
{
	return (!in_single_quotes);
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
	bool	in_single_quotes;
	bool	in_double_quotes;

	i = 0;
	in_single_quotes = false;
	in_double_quotes = false;
	while (input[i])
	{
		update_quote_state(input[i], &in_single_quotes, &in_double_quotes);
		if (input[i] == '$' && input[i + 1] == '?' && should_expand(in_single_quotes, in_double_quotes))
		{
			new_input = replace_exit_status(input, i);
			free(input);
			input = new_input;
			i = 0;
			in_single_quotes = false;
			in_double_quotes = false;
		}
		else if (input[i] == '$' && should_expand(in_single_quotes, in_double_quotes))
		{
			var = extract_var_name(input, i, &end);
			if (var == NULL)
			{
				i++;
				continue;
			}
			temp = getenv(var); // need to write a custom getenv to search the struct env list
			if (temp == NULL)
				temp = "";
			new_input = replace_var(input, i, end, temp);
			free(input);
			input = new_input;
			free(var);
			i = 0;
			in_single_quotes = false;
			in_double_quotes = false;
		}
		else
			i++;
	}
	return (input);
}
