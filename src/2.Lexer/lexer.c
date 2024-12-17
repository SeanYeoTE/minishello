/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:54:42 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/17 15:19:01 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 

static int	detect_operator(char *str)
{
	if ((*str == '|') || (*str == '>') || (*str == '<'))
		return (1);
	if ((*str == '(') || (*str == ')'))
		return (1);
	if ((*str == '+') || (*str == '-') || (*str == '%'))
		return (1);
	if ((*str == '~') || (*str == '?') || (*str == '*'))
		return (1);
	return (0);
}

bool	is_space(char c)
{
	if (c == 32 || c == 9 || c == 10 || c == 11 || c == 12 || c == 13)
		return (true);
	return (false);
}

static bool	check_redir_ambiguity(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] == '<' || str[i] == '>') && str[i + 1] && str[i - 1] != '-')
		{
			if (str[i + 1] == str[i])
				i += 2;
			else
				i++;
			while (is_space(str[i]))
				i++;
			if (str[i] == '$' && str[i + 1] && !is_space(str[i + 1])
				&& str[i + 1] != '/' && str[i + 1] != '$')
			{
				print_erroronly("ambiguous redirect", &str[i]);
				return (true);
			}
		}
		i++;
	}
	return (false);
}

int	full_lexer(char *str, t_shell *store, int index)
{
	if (str[index] != '\0')
	{
		if (str[index] == '#')
			return (full_lexer(str, store, scanner_comment(str, index, store)));
		else if (store->quotes == true || (str[index] == '"'
				|| str[index] == '\''))
			return (full_lexer(str, store, scanner_quote(str, index, store)));
		else if (detect_operator(&str[index]) == 1)
			return (full_lexer(str, store, scanner_operator(str, index,
						store)));
		else if (is_space(str[index]))
			return (full_lexer(str, store, scanner_space(str, index)));
		else
			return (full_lexer(str, store, scanner_word(str, index, store)));
	}
	return (0);
}

/**
 * @brief Prepares the input string for execution by handling various 
 * preprocessing steps
 * @param store Main shell data structure
 * @return EXIT_SUCCESS after successful preprocessing
 * @details Handles input spacing, variable expansions, lexical analysis,
 * and quote removal
 */
int	lexer(t_shell *store)
{
	store->input = input_spacer(store->input);
	if (ft_strchr(store->input, '$') != NULL)
	{
		if (check_redir_ambiguity(store->input))
		{
			store->exit_status = 1;
			return (EXIT_FAILURE);
		}
		store->expanded = true;
		store->input = expansions(store, store->input);
	}
	full_lexer(store->input, store, 0);
	remove_quote(store->head);
	return (EXIT_SUCCESS);
}
