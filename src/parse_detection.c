/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_detection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:54:42 by seayeo            #+#    #+#             */
/*   Updated: 2024/08/14 16:43:05 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// not checking for >> redirection to file
int	detect_operator(char *str)
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



int	full_lexer(char *str, t_shell *store, int index)
{
	if (str[index] != '\0')
	{
		if (str[index] == '#')
			return (full_lexer(str, store, scanner_comment(str, index, store)));
		else if (store->quotes == true || (str[index] == '"' || str[index] == '\''))
			return (full_lexer(str, store, scanner_quote(str, index, store)));
		else if (detect_operator(&str[index]) == 1)
			return (full_lexer(str, store, scanner_operator(str, index, store)));
		else if (str[index] == ' ')
			return (full_lexer(str, store, scanner_space(str, index)));
		else
			return (full_lexer(str, store, scanner_word(str, index, store)));
	}
	return (0);
}
