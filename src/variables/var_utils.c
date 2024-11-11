/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 23:47:14 by mchua             #+#    #+#             */
/*   Updated: 2024/11/08 23:47:14 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	name_counter(char *src)
{
	int		n;
	bool	checker;

	checker = false;
	n = 0;
	while (*src != '\0')
	{
		if (*src != '=')
			n++;
		else
		{
			checker = true;
			break ;
		}
		src++;
	}
	if (checker)
		return (n);
	else
		return (0);
}

static char	**ft_freeup(char **strs)
{
	int	i;

	if (strs)
	{
		i = 0;
		while (strs[i] != NULL)
		{
			free(strs[i]);
			i++;
		}
		free(strs);
	}
	return (NULL);
}

static char	*ft_word(char const *str, char c)
{
	int		len_word;
	int		i;
	char	*word;

	len_word = 0;
	i = 0;
	while (str[len_word] && !(str[len_word] == c))
		len_word++;
	word = (char *)malloc(sizeof(char) * (len_word + 1));
	if (!word)
		return (NULL);
	while (i < len_word)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split_var(char const *str, char c)
{
	char	**strings;
	int		i;

	i = 0;
	strings = (char **)malloc(sizeof(char *) * (3));
	if (!strings)
		return (NULL);
	if (*str != '\0' && *str != c)
	{
		strings[i] = ft_word(str, c);
		if (strings[i] == NULL)
			return (ft_freeup(strings));
		i++;
	}
	while (*str && !(*str == c))
		str++;
	str++;
	strings[i] = ft_strdup(str);
	if (strings[i] == NULL)
		return (ft_freeup(strings));
	i++;
	strings[i] = '\0';
	return (strings);
}

int	set_var(char *src, t_shell *store)
{
	t_var	*new_var;
	t_var	*current;

	new_var = NULL;
	if (!same_env(src, store) || !same_var(src, store))
		new_var = split_var(src);
	if (!new_var)
		return (EXIT_FAILURE);
	if (store->var == NULL)
		store->var = new_var;
	else
	{
		current = store->var;
		while (current)
		{
			if (current->next == NULL && new_var != NULL)
			{
				current->next = new_var;
				break ;
			}
			current = current->next;
		}
	}
	return (EXIT_SUCCESS);
}
