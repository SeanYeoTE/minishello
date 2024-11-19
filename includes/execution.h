/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:14:30 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/19 20:05:07 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"

// pipe_setup.c
int			pipe_counter(t_node *loop);
int			setup_pipe(int pipe_fds[2]);
void		handle_pipe_fds(int *in_fd, int pipe_fds[2], int is_last_cmd);
int			handle_command(t_shell *store, t_cmd *cmd, int *in_fd, int *out_fd);

// pipe_fd.c
void        close_fd_if_valid(int fd);
int			wait_for_command(t_shell *store, pid_t pid);
void		setup_pipes(int in_fd, int out_fd, t_cmd *cmd);

// pipe_process.c
void		run_cmd(t_cmd *cmd, t_shell *store);
int			execute_command(t_shell *store, t_cmd *cmd, int in_fd, int out_fd);
int			execute_and_wait(t_shell *store, t_cmd *cmd, int in_fd, int out_fd);

// pipe_exec.c
int			multi_executor(t_shell *store);
int			multiple_function(t_shell *store);

// single.c
int			single_function(t_shell *store, t_node *start, t_node *end);

// single_external.c
int         checkforheredoc(t_cmd *cmd);
int	        execute_external_command(t_shell *store, t_cmd *cmd);

// exec_path.c
char		*findprocesspath(t_shell *store, char **arr);
int			is_directory(const char *path);

// exec_argv.c
char		**argv_creator(t_node *start, t_node *end);
void		cleanup(char *exepath, char **argv, char *temp_filename);

// exec_fd.c
void		set_fd(t_cmd *node, char *temp_filename);
void		check_open_fds(int max_fd);

// exec_main.c
int			executor(t_shell *store, t_cmd *cmd);

#endif
