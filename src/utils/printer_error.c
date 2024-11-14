/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:02:30 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/14 12:55:32 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*create_error_prefix(char *arg)
{
	char	*prefix;

	prefix = ft_strjoin("bash: ", arg);
	return (prefix);
}

static char	*create_error_message(char *prefix, char *str)
{
	char	*tmp;
	char	*message;

	tmp = ft_strjoin(prefix, ": ");
	message = ft_strjoin(tmp, str);
	free(tmp);
	return (message);
}

int	print_error(char *str, char *arg)
{
	if (arg)
		printf("%s: %s\n", arg, str);
	else
		printf("minishell: %s\n", str);
	return (EXIT_FAILURE);
}

void	print_erroronly(char *str, char *arg)
{
	char	*prefix;
	char	*message;

	prefix = create_error_prefix(arg);
	message = create_error_message(prefix, str);
	free(prefix);
	ft_putstr_fd(message, STDERR_FILENO);
	free(message);
	ft_putchar_fd('\n', STDERR_FILENO);
}
