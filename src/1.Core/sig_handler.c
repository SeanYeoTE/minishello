/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:50:48 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 12:14:00 by seayeo           ###   ########.fr       */
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
	g_sig = 1;
	if (signum == SIGINT)
		write (STDERR_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	child_sigint_handler(int signum)
{
	if (signum)
	{
		g_sig = 1;
		write(STDERR_FILENO, "\n", 1);
		close(STDIN_FILENO);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_done = 1;
	}
}

// int	heredoc_sigint_handler(char *filename, t_shell *store)
// {
// 	(void)filename;
// 	free_all(store);
// 	g_sig = 0;
// 	return (130);
// }
// void	heredoc_setup_signals()
// {
// 	struct sigaction	action1;
// 	struct sigaction	action2;
// 	sigset_t			mask1;
// 	sigset_t			mask2;
// 	sigemptyset(&mask1);
// 	sigemptyset(&mask2);
// 	sigaddset(&mask1, SIGINT);
// 	sigaddset(&mask2, SIGQUIT);
// 	action1.sa_mask = mask1;
// 	action2.sa_mask = mask2;
// 	sigaction(SIGINT, &action1, NULL);
// 	sigaction(SIGQUIT, &action2, NULL);
// }
