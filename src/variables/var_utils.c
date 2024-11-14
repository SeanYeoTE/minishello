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

/**
 * @brief Counts the length of a variable name up to the equals sign
 *
 * This function counts characters in the string until it encounters an equals
 * sign or the end of the string. Returns 0 if no equals sign is found.
 *
 * @param src The source string containing the variable
 * @return int Length of the variable name, or 0 if no equals sign found
 */
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

/**
 * @brief Frees memory allocated for an array of strings
 *
 * Helper function that deallocates memory for an array of strings and its
 * contents, typically used for cleanup in case of allocation failures.
 *
 * @param strs Array of strings to free
 * @return char** Always returns NULL
 */
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

/**
 * @brief Extracts a word from a string up to a delimiter
 *
 * Creates a new string containing characters from the input string until
 * it encounters the specified delimiter character.
 *
 * @param str Input string to extract from
 * @param c Delimiter character
 * @return char* New string containing the extracted word
 */
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

/**
 * @brief Splits a variable string into name and value components
 *
 * Splits a string at the specified delimiter (typically '=') into two parts:
 * the variable name and its value. Returns an array of two strings.
 *
 * @param str String to split
 * @param c Delimiter character
 * @return char** Array containing the variable name and value
 */
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

/**
 * @brief Sets a variable in the shell's variable list
 *
 * Creates a new variable node and adds it to the shell's variable list if
 * it doesn't already exist in either the environment or variable list.
 *
 * @param src Source string containing the variable definition
 * @param store Shell structure containing variable and environment information
 * @return int EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
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
