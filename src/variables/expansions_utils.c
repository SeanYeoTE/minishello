/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:34:18 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/14 21:43:27 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Extracts a variable name from the input string
 *
 * @param input The input string containing the variable
 * @param start Starting position of the variable in the input string
 * @param end Pointer to store the ending position of the variable
 * @return char* The extracted variable name, or NULL if invalid
 */
char	*extract_var_name(const char *input, int start, int *end)
{
	int		j;
	char	*var_name;

	j = start + 1;
	if (!ft_isalpha(input[j]) && input[j] != '_')
	{
		*end = j;
		return (NULL);
	}
	while ((ft_isalnum(input[j]) || input[j] == '_') && input[j] != '\0')
		j++;
	*end = j;
	var_name = ft_strndup(input + start + 1, j - start - 1);
	return (var_name);
}

/**
 * @brief Updates the quote state based on the current character
 *
 * @param c Current character being processed
 * @param in_single_quotes Pointer to single quote state flag
 * @param in_double_quotes Pointer to double quote state flag
 */
void	update_quote_state(char c, bool *in_single_quotes,
	bool *in_double_quotes)
{
	if (c == '\'' && !*in_double_quotes)
		*in_single_quotes = !*in_single_quotes;
	else if (c == '"' && !*in_single_quotes)
		*in_double_quotes = !*in_double_quotes;
}

/**
 * @brief Determines if variable expansion should occur based on quote state
 *
 * @param in_single_quotes Current single quote state
 * @return bool True if expansion should occur, false otherwise
 */
bool	should_expand(bool in_single_quotes)
{
	return (!in_single_quotes);
}

/**
 * @brief Removes dollar quotes from the input string
 *
 * @param input The input string containing dollar quotes
 * @param start Starting position of the dollar quotes
 * @return char* A new string with dollar quotes removed
 */
char	*remove_dollar_quotes(char *input, int start)
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

/**
 * @brief Handles the export of environment variables
 *
 * @param store Shell structure containing environment variables
 * @param current_arg The argument to be exported
 * @return int Return value indicating success (0) or failure
 */
int	handle_export(t_shell *store, char *current_arg)
{
	char	*arg;
	int		ret_value;

	ret_value = 0;
	arg = current_arg;
	ret_value = check_arg(arg);
	if (!ft_strchr (arg, '=') && ret_value == 0)
		return (0);
	if (ft_strchr (arg, '=') && ret_value == 3)
		return (0);
	print_error_msg(arg, ret_value);
	if (ret_value == 0)
		set_export(store, arg);
	return (ret_value);
}
