/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:12:10 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/15 18:02:31 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORE_H
# define CORE_H

# include "minishell.h"

// main.c
void		init_shell_state(t_shell_state *state, t_env *env_head);
int			main(int argc, char **argv, char **envp);

// base.c
int			minishell_loop(t_shell *store, t_shell_state *state);

// setup_utils.c
void		init_var(t_shell *store, t_shell_state *state);
void		update_shell_state(t_shell *store, t_shell_state *state);
void		prompter_init(char **prompt);
int			prompter_input(t_shell *store, char *prompt);

// setup_getters.c
char		*ft_getenv(char *var, t_env *env);
char		**ft_createarray(t_env *env);
char		*form_prompt(char *cwd);

// sig_handler.c
void		ctrl_c_handler(int signum);
int			heredoc_sigint_handler(char *filename, t_shell *store);
void		heredoc_setup_signals();
void		child_sigint_handler(int signum);

// exit_wrapper.c
void		exit_wrapper(t_shell *store, int exit_status);

#endif
