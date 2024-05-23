/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_detection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 15:54:42 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/23 13:54:19 by seayeo           ###   ########.fr       */
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
	return (start + i);
}

int	scanner_quote(char *str, int start, t_shell *store)
{
	int		i;
	int		count;
	char	*data;
	
	i = 0;
	count = 0;
	while (str[start + i])
	{
		if (str[start + i] == '"')
		{
			count++;
			if (count == 2)
				break;
		}
		i++;
	}
	ft_strlcpy(data, str + start + 1, i + 1);
	init_node(data, &store->head);
	get_last(store->head)->type = 2;
	return (start + i);
}

int	scanner_operator(char *str, int start, t_shell *store)
{
	int		i;
	char	*data;

	i = 0;
	while (str[start + i])
	{
		if (str[start + i] == '<' || str[start + i] == '>') // to confirm which other operator we should add in
			break;
		i++;
	}
	ft_strlcpy(data, str + start, i + 1);
	init_node(data, &store->head);
	get_last(store->head)->type = 3;
	return (start + i);
}

int	scanner_space(char *str, int start, t_shell *store)
{
	int		i;
	char	*data;

	i = 0;
	while (str[start + i])
	{
		if (str[start + i] != ' ') //do we need to include whitespaces?
			break;
		i++;
	}
	// ft_strlcpy(data, str + start, i);
	// init_node(data, &store->head);
	// get_last(store->head)->type = 4;
	return (start + i);
}

int scanner_word(char *str, int start, t_shell *store)
{
	int		i;
	char	*data;

	i = 0;
	// puts("entry");
	while (str[start + i])
	{
		i++;
		if (str[start + i] == ' ')
			break;
	}
	ft_strlcpy(data, str + start , i + 1);
	init_node(data, &store->head);
	get_last(store->head)->type = 5;
	return (start + i);
}


int ft_sscan(char *str, t_shell *store, int index)
{
	printf("index: %d\n", index);
	printf("char: %c\n", str[index]);
	if (str[index] != '\0')
	{	
		puts("wtf");
		if (str[index] == '#')
			return ft_sscan(str, store, scanner_comment(str, index, store));
		else if (str[index] == '"')
			return ft_sscan(str, store, scanner_quote(str, index, store));
		else if (detect_operator(&str[index]) == 1)
			return ft_sscan(str, store, scanner_operator(str, index, store));
		else if (str[index] == ' ')
			return ft_sscan(str, store, scanner_space(str, index, store));
		else
			return ft_sscan(str, store, scanner_word(str, index, store));
	}
	return (0);
}


int	ft_scanner(char *str, t_shell *store)
{
	int i;
	
	i = 0;
	// printf("*str = %s\n", str);
	// puts("hit");
	while (str[i] != '\0')
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
		i++;
	}   
	return (0);
}
