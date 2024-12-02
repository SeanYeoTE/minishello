/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_replace.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:34:18 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 12:16:30 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Calculates the length needed for the result string after variable
 * replacement
 * @param front The string before the variable
 * @param back The string after the variable
 * @param value The value to replace the variable with
 * @return size_t The total length needed for the result string
 */
static size_t	calculate_result_len(char *front, char *back, const char *value)
{
	size_t	result_len;

	result_len = ft_strlen(value) + 1;
	if (front)
		result_len += ft_strlen(front);
	if (back)
		result_len += ft_strlen(back);
	return (result_len);
}

/**
 * @brief Replaces a variable in a string with its value
 *
 * @param input The original input string
 * @param start Starting index of the variable in the input string
 * @param end Ending index of the variable in the input string
 * @param value The value to replace the variable with
 * @return char* A new string with the variable replaced
 */
char	*replace_var(char *input, int start, int end, const char *value)
{
	char	*front;
	char	*back;
	char	*result;
	size_t	result_len;

	front = ft_strndup(input, start);
	back = ft_strdup(input + end);
	result_len = calculate_result_len(front, back, value);
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

/**
 * @brief Replaces the $? variable with the last exit status
 *
 * @param store Shell structure containing the exit status
 * @param input The original input string
 * @param start Starting index of the $? in the input string
 * @return char* A new string with the exit status replaced
 */
char	*replace_exit_status(t_shell *store, char *input, int start)
{
	char	*front;
	char	*back;
	char	*result;
	char	*exit_str;
	size_t	result_len;

	front = ft_strndup(input, start);
	back = ft_strdup(input + start + 2);
	exit_str = ft_itoa(store->exit_status);
	result_len = calculate_result_len(front, back, exit_str);
	result = (char *)malloc(result_len);
	result[0] = '\0';
	if (result)
	{
		if (front != NULL)
			ft_strlcpy(result, front, ft_strlen(front) + 1);
		ft_strlcat(result, exit_str, result_len);
		if (back != NULL)
			ft_strlcat(result, back, result_len);
	}
	free(front);
	free(back);
	free(exit_str);
	return (result);
}
