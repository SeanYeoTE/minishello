/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:16:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/14 20:04:28 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

# include "minishell.h"

// redir_handler.c
int			redir_handler(t_shell *store, t_cmd *cmd, t_node *loop, t_node *end);

// redir_io.c
int			handle_output_redirection(t_cmd *cmd, char *filename);
int			handle_append_redirection(t_cmd *cmd, char *filename);
int			handle_input_redirection(t_cmd *cmd, char *filename);

// redir_utils.c
void		reset_fds(t_shell *store, int check);

// heredoc_wrapper.c
int			heredoc_finisher(t_cmd *cmd, t_shell *store, int child);
int			handle_all_heredocs(t_shell *store);
int			heredoc_child(t_cmd *cmd, t_shell *store, int child2);

// heredoc.c
int			heredoc_single_external(t_cmd *cmd, t_shell *store);
int			heredoc_single_builtin(t_cmd *cmd, t_shell *store);

#endif
