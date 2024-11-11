/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:13:20 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 14:11:42 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "minishell.h"

// builtin_main.c
int			builtin_main(t_shell *store, t_node *current);

// cd.c
int			cd_handler(t_node *current);

// echo.c
int			echo_handler(t_node *current);

// pwd.c
int			pwd_handler(void);

// env.c
t_env		*get_env_loc(t_env *env_list, char *arg);
t_var		*get_var_loc(char *arg, t_var *var_list, t_env *current_env);
t_env		*create_env_node(char *var);
t_env		*env_init(char **envp);
int			env_handler(t_shell *store);

// export.c
int			check_arg(char *arg);
void		set_export(t_shell *store, char *arg);
int			export_handler(t_shell *store, t_node *args);

// export_utils.c
void		print_error_msg(char *arg, int ret_value);
void		sort_env(char **str_array, int env_len);
void		print_export(char **str_arr, int env_len);
bool		is_in_env(t_env *env_list, char *arg);
t_env		*get_last_env(t_env *current_env);

// unset.c
int			unset_handler(t_shell *store, t_node *current);

// exit.c
int			exit_handler(t_shell *store);

#endif
