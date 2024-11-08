/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 12:54:43 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/08 13:55:04 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

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
	if (ft_strcmp(loop->data, "export") == 0)
		return (1);
	if (ft_strcmp(loop->data, "echo") == 0)
		return (1);
	if (ft_strcmp(loop->data, "cd") == 0)
		return (1);
	if (ft_strcmp(loop->data, "pwd") == 0)
		return (1);
	if (ft_strcmp(loop->data, "unset") == 0)
		return (1);
	if (ft_strchr(loop->data, '=') != NULL)
		return (1);
	if (ft_strcmp(loop->data, "exit") == 0)
		return (1);
	if (loop->next != NULL)
		return (0);
	if (ft_strcmp(loop->data, "env") == 0)
		return (1);
	else
		return (0);
}

int	is_operator(char c)
{
	return (c == '>' || c == '<' || c == '|');
}

int	is_double_operator(const char *input, int i)
{
	if (input[i] == '>' && input[i + 1] == '>')
		return (1);
	if (input[i] == '<' && input[i + 1] == '<')
		return (1);
	return (0);
}
