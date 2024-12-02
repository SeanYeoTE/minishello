/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:16:50 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 16:30:35 by seayeo           ###   ########.fr       */
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

// heredoc_core.c
int			expandforheredoc(t_cmd *cmd);
int			exec_heredoc(t_cmd *cmd, t_shell *store, int is_last_heredoc);

// heredoc_setup.c
int			setup_heredoc_pipes(t_cmd *cmd);
int			setup_heredoc_pipe(int pipe_fds[2]);
void		cleanup_heredoc_fd(t_cmd *cmd);
void		cleanup_write_fds(t_shell *store);
int			setup_all_heredoc_pipes(t_shell *store);

// heredoc_process.c
int			handle_child_heredoc(t_cmd *cmd, t_shell *store, int pipe_fds[2]);
int			process_heredocs(t_shell *store);
int			heredoc_child(t_cmd *cmd, t_shell *store);
int			heredoc_child_loop(t_shell *store);

// heredoc_utils.c
int			is_last_heredoc(t_node *current);
void		cleanup_delimiter(t_cmd *cmd);
int			process_heredoc_node(t_cmd *cmd, t_shell *store, t_node *node);
int			heredoc_finisher(t_cmd *cmd, t_shell *store);

#endif
