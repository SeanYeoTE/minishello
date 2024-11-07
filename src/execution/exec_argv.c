/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_argv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/07 22:26:40 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

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

static char	**init_argv(int size)
{
	char	**ret;

	ret = (char **)ft_calloc(size + 2, sizeof(char *));
	if (!ret)
		return (NULL);
	return (ret);
}

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
