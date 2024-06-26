/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 13:31:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/26 11:28:34 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// introduces spaces between operators and strings
char	*input_spacer(char *input)
{
	int		i;
	char	*front;
	char	*back;
	int		detected;

	i = 0;
	detected = 0;
	while (input[i])
	{
		if (input[i] == '<' || input[i] == '>')
			detected = 1;
		else
		{
			if (detected == 1)
			{
				front = ft_substr(input, 0, i);
				back = ft_substr(input, i, ft_strlen(input) - i);
				free(input);
				input = ft_strjoin(ft_strjoin(front, " "), back);
				free(front);
				free(back);
				i++;
			}
			detected = 0;
		}
		i++;
	}
	return (input);
}

// 34 is double quote, 39 is single quote
// returns 1 if all quotes are closed, 0 if not
int	check_quotes(char *line)
{
	puts("check_quotes");
	int	i;
	int	sml;
	int	dbl;

	sml = 0;
	dbl = 0;
	while (line[i])
	{
		if (line[i] == 34 && !sml)
			dbl = !dbl;
		if (line[i] == 39 && !dbl)
			sml = !dbl;
		i++;
	}
	return (!(sml || dbl));
}

int redir_checker(t_node *cmd)
{
	if (ft_strcmp(cmd->data, ">") == 0)
		return (1);
	if (ft_strcmp(cmd->data, ">>") == 0)
		return (1);
	if (ft_strcmp(cmd->data, "<") == 0)
		return (1);
	if (ft_strcmp(cmd->data, "<<") == 0)
		return (1);
	return (0);
}