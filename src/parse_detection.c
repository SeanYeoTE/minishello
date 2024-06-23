/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_detection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:54:42 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/23 14:50:36 by seayeo           ###   ########.fr       */
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
	else
		return (0);
}

int redir_checker(t_node *loop)
{
	while (loop)
	{
		if (ft_strcmp(loop->data, ">") == 0)
			return (1);
		if (ft_strcmp(loop->data, ">>") == 0)
			return (1);
		if (ft_strcmp(loop->data, "<") == 0)
			return (1);
		if (ft_strcmp(loop->data, "<<") == 0)
			return (1);
		loop = loop->next;
	}
	return (0);
}

int	ft_sscan(char *str, t_shell *store, int index)
{
	if (str[index] != '\0')
	{
		if (str[index] == '#')
			return (ft_sscan(str, store, scanner_comment(str, index, store)));
		else if (str[index] == '"')
			return (ft_sscan(str, store, scanner_quote(str, index, store)));
		else if (detect_operator(&str[index]) == 1)
			return (ft_sscan(str, store, scanner_operator(str, index, store)));
		else if (str[index] == ' ')
			return (ft_sscan(str, store, scanner_space(str, index)));
		else
			return (ft_sscan(str, store, scanner_word(str, index, store)));
	}
	return (0);
}
