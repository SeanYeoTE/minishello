/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:54:43 by seayeo            #+#    #+#             */
/*   Updated: 2024/08/09 12:59:08 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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
	if (ft_strcmp(loop->data, "env") == 0)
		return (1);
	if (ft_strchr(loop->data,  '=') != NULL)
		return (1);
	else
		return (0);
}

int	is_operator(char c)
{
	return (c == '>' || c == '<' || c == '|' || c == '=');
}

int	is_double_operator(const char *input, int i)
{
	if (input[i] == '>' && input[i + 1] == '>')
		return 1;
	if (input[i] == '<' && input[i + 1] == '<')
		return 1;
	return 0;
}