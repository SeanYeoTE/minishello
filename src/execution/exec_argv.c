/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 14:28:35 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 
/**
 * @brief Counts the number of nodes between start and end nodes
 *
 * @param start Pointer to the starting node
 * @param end Pointer to the ending node (exclusive)
 * @return int Number of nodes between start and end
 */
static int	count_args(t_node *start, t_node *end)
{
	int		count;
	t_node	*temp;

	count = 0;
	temp = start;
	while (temp && temp != end)
	{
		temp = temp->next;
		count++;
	}
	return (count);
}

/**
 * @brief Initializes an array of strings with given size
 *
 * @param size Number of arguments to allocate space for
 * @return char** Newly allocated array of strings, NULL if allocation fails
 * @note Allocates size + 2 to account for program name and NULL terminator
 */
static char	**init_argv(int size)
{
	char	**ret;

	ret = (char **)ft_calloc(size + 2, sizeof(char *));
	if (!ret)
		return (NULL);
	return (ret);
}

/**
 * @brief Fills the argv array with strings from linked list nodes
 *
 * @param ret Array to fill with strings
 * @param start Starting node of the linked list
 * @param end Ending node of the linked list (exclusive)
 * @return int 0 on success, -1 on failure
 */
static int	fill_argv(char **ret, t_node *start, t_node *end)
{
	int	i;

	i = 0;
	while (start && start != end)
	{
		ret[i] = ft_strdup(start->data);
		if (!ret[i])
			return (-1);
		start = start->next;
		i++;
	}
	ret[i] = NULL;
	return (0);
}

/**
 * @brief Creates an array of strings from linked list nodes
 *
 * @param start Starting node of the linked list
 * @param end Ending node of the linked list (exclusive)
 * @return char** Array of strings containing node data, NULL if allocation fails
 * @note Caller is responsible for freeing the returned array
 */
char	**argv_creator(t_node *start, t_node *end)
{
	char	**ret;
	int		arg_count;
	int		i;

	arg_count = count_args(start, end);
	ret = init_argv(arg_count);
	if (!ret)
		return (NULL);
	if (fill_argv(ret, start, end) == -1)
	{
		i = 0;
		while (ret[i])
			free(ret[i++]);
		free(ret);
		return (NULL);
	}
	return (ret);
}

/**
 * @brief Cleans up allocated memory for execution
 *
 * @param exepath Path to executable to be freed
 * @param argv Array of argument strings to be freed
 * @param temp_filename Temporary filename (currently unused)
 */
void	cleanup(char *exepath, char **argv, char *temp_filename)
{
	int	i;

	if (exepath)
		free(exepath);
	if (argv)
	{
		i = 0;
		while (argv[i])
			free(argv[i++]);
		free(argv);
	}
	(void)temp_filename;
}
