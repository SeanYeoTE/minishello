/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_detection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:54:42 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/20 21:09:03 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	detect_operator(char *str)
{
	if ((*str == '|') || (*str == '>') || (*str == '<') || (*str == '>>'))
		return (1);
	if ((*str == '(') || (*str == ')'))
		return (1);
	if ((*str == '+') || (*str == '-') || (*str == '/') || (*str == '%'))
		return (1);
	if ((*str == '~') || (*str == '?') || (*str == '*'))
		return (1);
	return (0);
}

int	scanner_comment(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == "\n")
			return (i);
		i++;
	}
	return (i);
}

int	scanner_quote(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
		if (str[i] == '"')
			return (i++);
	}
	return (i);
}

int	scanner_operator(char *str)
{
		int	i;

	i = 0;
	while (str[i])
	{
		i++;
		if (str[i] == '<' || str[i] == '>') // to confirm which other operator we should add in
			return (i++);
	}
	return (i);
}

int	scannner_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
		if (str[i] >= ' ') //do we need to include whitespaces?
			return (i++);
	}
	return (i);
}

int scanner_word(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
		if (str[i] == ' ' || str[i] == "\0")
			return (i++);
	}
	return (i);
}


char	**ft_scanner(char const *str, t_shell *store)
{
	int i;
	
	i = 0;
	while (str[i])
	{
		if (str[i] == '#')
			i += scanner_comment(str);
		else if (str[i] == '"')
			i +=  scanner_quote(str[i]);
		else if (detect_operator(&str[i]) == 1)
			i += scanner_operator(str[i]);
		else if (str[i] == ' ')
			i += scanner_space(str[i]);
		else
			i += scanner_word(str[i]);
		i++;
	}    
}
