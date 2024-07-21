/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_detection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:54:42 by seayeo            #+#    #+#             */
/*   Updated: 2024/07/21 16:47:56 by mchua            ###   ########.fr       */
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

int	check_builtin(t_node *loop)
{
	if (ft_strcmp(loop->data, "echo") == 0)
		return (1);
	if (ft_strcmp(loop->data, "cd") == 0)
		return (1);
	if (ft_strcmp(loop->data, "pwd") == 0)
		return (1);	
	if (ft_strcmp(loop->data, "export") == 0)
		return (1);
	if (ft_strcmp(loop->data, "env") == 0)
		return (1);
	if (ft_strchr(loop->data,  '=') != NULL)
		return (1);
	else
		return (0);
}

int	full_lexer(char *str, t_shell *store, int index)
{
	if (str[index] != '\0')
	{
		if (str[index] == '#')
			return (full_lexer(str, store, scanner_comment(str, index, store)));
		else if (str[index] == '"')
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
