/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_detection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:54:42 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/21 17:42:56 by seayeo           ###   ########.fr       */
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
	if ((*str == '+') || (*str == '-') || (*str == '/') || (*str == '%'))
		return (1);
	if ((*str == '~') || (*str == '?') || (*str == '*'))
		return (1);
	return (0);
}

int	scanner_comment(char *str, int start, t_shell *store)
{
	int		i;
	char	*data;
	
	i = 0;
	while (str[start + i])
	{
		if (str[start + i] == '\n')
			return (start + i);
		i++;
	}
	ft_strlcpy(data, str + start, i);
	init_node(data, &store->head);
	get_last(store->head)->type = 1;
	return (i);
}

int	scanner_quote(char *str, int start, t_shell *store)
{
	int		i;
	char	*data;
	
	i = 0;
	while (str[start + i])
	{
		i++;
		if (str[start + i] == '"')
			return (i++);
	}
	ft_strlcpy(data, str + start, i);
	init_node(data, &store->head);
	get_last(store->head)->type = 2;
	return (i);
}

int	scanner_operator(char *str, int start, t_shell *store)
{
	int		i;
	char	*data;

	i = 0;
	while (str[i])
	{
		i++;
		if (str[i] == '<' || str[i] == '>') // to confirm which other operator we should add in
			return (i++);
	}
	ft_strlcpy(data, str + start, i);
	init_node(data, &store->head);
	get_last(store->head)->type = 3;
	return (i);
}

int	scanner_space(char *str, int start, t_shell *store)
{
	int		i;
	char	*data;

	i = 0;
	while (str[start + i])
	{
		i++;
		if (str[start + i] >= ' ') //do we need to include whitespaces?
			return (i++);
	}
	ft_strlcpy(data, str + start, i);
	init_node(data, &store->head);
	get_last(store->head)->type = 1;
	return (i);
}

int scanner_word(char *str, int start, t_shell *store)
{
	int		i;
	char	*data;

	i = 0;
	puts("entry");
	while (str[i])
	{
		i++;
		if (str[i] == ' ' || str[i] == '\0')
		{
			i++;
			break;	
		}
	}
	ft_strlcpy(data, str + start , i);
	printf("data = %s\n", data);
	init_node(data, &store->head);
	get_last(store->head)->type = 1;
	return (i);
}


int	ft_scanner(char *str, t_shell *store)
{
	int i;
	
	i = 0;
	// printf("*str = %s\n", str);
	// puts("hit");
	while (str[i])
	{
		// printf("before %c\n", str[i]);
		if (str[i] == '#')
			i += scanner_comment(str, i, store);
		else if (str[i] == '"')
			i +=  scanner_quote(str, i, store);
		else if (detect_operator(&str[i]) == 1)
			i += scanner_operator(str, i, store);
		else if (str[i] == ' ')
			i += scanner_space(str, i, store);
		else
			i += scanner_word(str, i, store);
		// printf("after %c\n", str[i]);
		// printf("%d\n", i);
		// if (str[i] != '\0')
		// 	i++;
		
	}   
	return (0);
}
