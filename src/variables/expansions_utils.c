/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 15:34:18 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 14:28:35 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 
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

void	update_quote_state(char c, bool *in_single_quotes,
	bool *in_double_quotes)
{
	if (c == '\'' && !*in_double_quotes)
		*in_single_quotes = !*in_single_quotes;
	else if (c == '"' && !*in_single_quotes)
		*in_double_quotes = !*in_double_quotes;
}

bool	should_expand(bool in_single_quotes)
{
	return (!in_single_quotes);
}

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

int	handle_export(t_shell *store, char *current_arg)
{
	char	*arg;
	int		ret_value;

	ret_value = 0;
	arg = current_arg;
	ret_value = check_arg(arg);
	if (!ft_strchr (arg, '='))
		return (0);
	print_error_msg(arg, ret_value);
	if (ret_value == 0)
		set_export(store, arg);
	return (ret_value);
}
