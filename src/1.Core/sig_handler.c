/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:50:48 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/10 19:45:28 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Signal handler for SIGINT (Ctrl+C) interrupts
 * @param signum Signal number (SIGINT in this case)
 * @details Handles Ctrl+C interrupts by:
 *          1. Writing a newline to stderr
 *          2. Clearing the current input line
 *          3. Moving to a new prompt line
 *          4. Redisplaying the prompt
 *          This provides a clean interrupt experience while preserving
 *          shell functionality
 */
void	ctrl_c_handler(int signum)
{
	if (signum == SIGINT)
		write (STDERR_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
