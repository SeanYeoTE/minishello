/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:50:48 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 23:06:23 by mchua            ###   ########.fr       */
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

void	child_sigint_handler(int signum)
{
	if (signum)
		g_sig = 1;
	puts("SIGINT detected\n");
}

int	heredoc_sigint_handler(char *filename, t_shell *store)
{
	free (filename);
	free_all (store);
	g_sig = 0;
	exit (130);
	
}
void	heredoc_setup_signals()
{
	struct sigaction	action1;
	struct sigaction	action2;
	sigset_t			mask1;
	sigset_t			mask2;
	
	sigemptyset(&mask1);
	sigemptyset(&mask2);
	sigaddset(&mask1, SIGINT);
	sigaddset(&mask2, SIGQUIT);
	action1.sa_mask = mask1;
	action2.sa_mask = mask2;
	action1.sa_handler = child_sigint_handler;
	action1.sa_flags = 0;
	action2.sa_handler = SIG_IGN;
	action2.sa_flags = 0;
	sigaction(SIGINT, &action1, NULL);
	sigaction(SIGQUIT, &action2, NULL);
}
