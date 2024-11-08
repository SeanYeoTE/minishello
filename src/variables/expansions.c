/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 15:34:18 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/09 02:51:28 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

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

static bool	should_expand(bool in_single_quotes)
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
	result = (char *)ft_calloc(result_len, sizeof(char));
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
	result_len = ft_strlen(ft_itoa(g_exit_status)) + 1;
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
		ft_strlcat(result, ft_itoa(g_exit_status), result_len);
		if (back != NULL)
			ft_strlcat(result, back, result_len);
	}
	free(front);
	free(back);
	return (result);
}

// Updated function to handle $"..." syntax with variable expansion
static char	*remove_dollar_quotes(char *input, int start)
{
	int		end;
	char	*front;
	char	*content;
	char	*back;
	char	*result;

	end = start + 2;
	while (input[end] && input[end] != '"')
		end++;
	if (input[end] != '"')
		return (input);
	front = ft_strndup(input, start);
	content = ft_strndup(input + start + 2, end - start - 2);
	back = ft_strdup(input + end + 1);
	result = ft_strjoin(front, content);
	free(front);
	free(content);
	front = result;
	result = ft_strjoin(front, back);
	free(front);
	free(back);
	free(input);
	return (result);
}

// Main function to expand variables
char	*expansions(t_shell *store, char *input)
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
		// printf("input: %s\n", input);
		update_quote_state(input[i], &in_single_quotes, &in_double_quotes);
		if (in_single_quotes)
		{
			i++;
			continue;
		}
		if (input[i] == '"' || input[i] == '\'')
			if (input[i] == input[i + 1])
			{
				i++;
				continue;
			}
		if (input[i] == '$' && input[i + 1] == '"' && !in_double_quotes)
		{
			input = remove_dollar_quotes(input, i);
			i = 0;  // Reset to reprocess from the beginning
			in_single_quotes = false;
			in_double_quotes = false;
			continue;
		}
		if (input[i] == '$' && input[i + 1] == '?' && should_expand(in_single_quotes))
		{
			new_input = replace_exit_status(input, i);
			free(input);
			input = new_input;
			i = 0;
			in_single_quotes = false;
			in_double_quotes = false;
		}
		else if (input[i] == '$' && should_expand(in_single_quotes))
		{
			var = extract_var_name(input, i, &end);
			if (var == NULL)
			{
				i++;
				continue;
			}
			temp = cgetenv(var, store->env); // need to write a custom getenv to search the struct env list
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
