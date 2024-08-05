/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 13:31:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/08/05 16:38:39 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*settle_front(char *input, int i)
{
	char	*front;
	char	*ans;

	front = ft_substr(input, 0, i);
	if (input[i - 1] == ' ')
	{
		front = ft_substr(input, 0, i);
		ans = ft_strjoin(front, ft_substr(input, i, 1));
	}
	else
	{
		front = ft_substr(input, 0, i);
		ans = ft_strjoin(ft_strjoin(front, " "), ft_substr(input, i, 1));
	}
	return (free(front), ans);
}

char	*settle_back(char *input, int i)
{
	char	*back;
	char	*ans;

	if (input[i + 1] == ' ')
	{
		back = ft_substr(input, i + 1, ft_strlen(input) - i);
		ans = ft_strdup(back);
	}
	else
	{
		back = ft_substr(input, i + 1, ft_strlen(input) - i);
		ans = ft_strjoin(" ", back);
	}
	return (free(back), ans);
}

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
		if (input[i] == '<' || input[i] == '>' || input[i] == '|'
			|| input[i] == '=')
			detected = 1;
		if (detected == 1)
		{
			front = settle_front(input, i);
			back = settle_back(input, i);
			input = ft_strjoin(front, back);
			free(front);
			free(back);
			i++;
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
	int	i;
	int	sml;
	int	dbl;

	sml = 0;
	dbl = 0;
	i = 0;
	while (line[i])
	{
		if (line[i] == 34 && sml != 1)
			dbl = !dbl;
		if (line[i] == 39 && dbl != 1)
			sml = !sml;
		i++;
	}
	return (!(sml || dbl));
}

int	redir_checker(t_node *cmd)
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
