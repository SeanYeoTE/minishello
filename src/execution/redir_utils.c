/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 21:54:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/07 21:58:12 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

bool	are_same_resource(int fd1, int fd2)
{
	struct stat	stat1;
	struct stat	stat2;

	if (fstat(fd1, &stat1) == -1)
	{
		perror("fstat");
		return (false);
	}
	if (fstat(fd2, &stat2) == -1)
	{
		perror("fstat");
		return (false);
	}
	return ((stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino));
}

void	reset_fds(t_shell *store)
{
	if (!are_same_resource(store->input_reset, STDIN_FILENO))
	{
		if (dup2(store->input_reset, STDIN_FILENO) == -1)
			print_erroronly("dup2 failed on input reset", strerror(errno));
	}
	if (!are_same_resource(store->output_reset, STDOUT_FILENO))
	{
		if (dup2(store->output_reset, STDOUT_FILENO) == -1)
			print_erroronly("dup2 failed on output reset", strerror(errno));
	}
}
