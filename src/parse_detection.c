/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_detection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:54:42 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/20 17:37:22 by seayeo           ###   ########.fr       */
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
			return (i);	
	}
	return (i);
}

int	scanner_operator(char *str)
{
	
}
int	scannner_space(char *str)
{
	
}

int scanner_word(char *str)
{
	
}



char	**ft_scanner(char const *str, t_shell *store)
{
	int i;
	
	i = 0;
	while (str[i])
	{
		if (str[i] == '#')
			i = scanner_comment(str);
		else if (str[i] == '"')
			i =  scanner_quote(str);
		else if (detect_operator(&str[i]) == 1)
			i = scanner_operator(str);
		else if (str[i] == ' ')
			i = scanner_space(str);
		else
			i = scanner_word(str);
		i++;
	}    
}
