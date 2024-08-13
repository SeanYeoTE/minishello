/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 15:34:18 by seayeo            #+#    #+#             */
/*   Updated: 2024/08/13 14:55:05 by seayeo           ###   ########.fr       */
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

// Helper function to replace variable with its value
static char	*replace_var(char *input, int start, int end, const char *value)
{
	char	*front;
	char	*back;
	size_t	result_len;
	char	*result;

	result_len = 0;
	printf("input: %s\n", input);
	printf("start: %d\n", start);
	printf("%c\n", input[start]);
	front = ft_strndup(input, start);
	printf("front: %s\n", front);
	back = ft_strdup(input + end);
	printf("back: %s\n", back);
	result_len = ft_strlen(value) + 1;
	if (front)
		result_len += ft_strlen(front);
	if (back)
		result_len += ft_strlen(back);
	// result_len = ft_strlen(front) + ft_strlen(value) + ft_strlen(back) + 1;
	printf("result_len: %zu\n", result_len);
	result = malloc(result_len);
	if (result)
	{
		if (front != NULL)
			ft_strlcpy(result, front, ft_strlen(front) + 1);
		printf("result: %s\n", result);
		ft_strlcat(result, value, ft_strlen(value) + 1);
		printf("result: %s\n", result);
		if (back != NULL)
			ft_strlcat(result, back, ft_strlen(back) + 1);
		printf("result: %s\n", result);
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
		if (input[i] == '$')
		{
			var = extract_var_name(input, i, &end);
			temp = getenv(var);
			if (temp == NULL)
				temp = "";
			new_input = replace_var(input, i, end, temp);
			free(input);
			input = new_input;
			free(var);
		}
		i++;
	}
	return (input);
}
