/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:16:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/15 14:31:50 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

# include "minishell.h"

// redir_handler.c
int			redir_handler(t_shell *store, t_cmd *cmd,
				t_node *loop, t_node *end);

// redir_io.c
int			handle_output_redirection(t_cmd *cmd, char *filename);
int			handle_append_redirection(t_cmd *cmd, char *filename);
int			handle_input_redirection(t_cmd *cmd, char *filename);

// redir_utils.c
void		reset_fds(t_shell *store, int check);
bool		are_same_resource(int fd1, int fd2);

// heredoc_utils.c
void		close_heredoc_write(t_cmd *cmd);
int			setup_heredoc_pipes(t_cmd *cmd);
int			is_last_heredoc(t_node *current);
int			setup_heredoc_pipes_wrapper(t_shell *store);
void		heredoc_finisher_wrapper(t_shell *store);

// heredoc_core.c
int			exec_heredoc(t_cmd *cmd, t_shell *store, int is_last_heredoc);
int			heredoc_finisher(t_cmd *cmd, t_shell *store);

// heredoc_wrapper.c
int			heredoc_finisher(t_cmd *cmd, t_shell *store);
int			heredoc_child(t_cmd *cmd, t_shell *store);
int			heredoc_child_loop(t_shell *store);

#endif
