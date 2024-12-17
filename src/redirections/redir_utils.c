/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 21:54:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/15 14:31:23 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 

/**
 * @brief Checks if two file descriptors point to the same resource
 *
 * @param fd1 First file descriptor to compare
 * @param fd2 Second file descriptor to compare
 * @return bool true if both fds point to same resource, false otherwise
 * @note Compares device ID and inode number to determine if same resource
 *       Returns false if either fstat call fails
 */
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

/**
 * @brief Resets standard input and output to their original file descriptors
 *
 * @param store Shell data structure containing reset file descriptors
 * @note Only resets if current stdin/stdout differ from original
 *       Prints error message if dup2 fails during reset
 */
void	reset_fds(t_shell *store, int check)
{
	if (check == 1)
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
	else
	{
		if (dup2(store->input_reset, STDIN_FILENO) == -1)
			print_erroronly("dup2 failed on input reset", strerror(errno));
		if (dup2(store->output_reset, STDOUT_FILENO) == -1)
			print_erroronly("dup2 failed on output reset", strerror(errno));
	}
}
