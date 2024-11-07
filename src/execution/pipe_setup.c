/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:26:54 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/07 22:36:14 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

int	pipe_counter(t_node *loop)
{
	int	count;

	count = 0;
	while (loop)
	{
		if (ft_strcmp(loop->data, "|") == 0)
			count++;
		loop = loop->next;
	}
	return (count);
}

int	setup_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
	{
		print_error("Pipe failed", strerror(errno));
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void	handle_pipe_fds(int *in_fd, int pipe_fds[2], int is_last_cmd)
{
	if (*in_fd > 2)
		close(*in_fd);
	if (!is_last_cmd)
	{
		if (pipe_fds[1] > 2)
			close(pipe_fds[1]);
		*in_fd = pipe_fds[0];
	}
	else
	{
		if (pipe_fds[0] > 2)
			close(pipe_fds[0]);
		if (pipe_fds[1] > 2)
			close(pipe_fds[1]);
	}
}
