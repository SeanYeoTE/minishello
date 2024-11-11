/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:18:00 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 15:28:43 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

// printer.c
int			print_stack(t_node **head);
int			print_stack_se(t_node *start, t_node *end);
int			print_argv(char **argv);
int			print_cmd_stack(t_cmd **head);

// printer_error.c
int			print_error(char *str, char *arg);
void		print_erroronly(char *str, char *arg);

// mem_utils.c
void		freechararray(char **v);
void		free_stack(t_node **stack);
void		free_cmd(t_cmd **cmd);

// mem_utils_extra.c
void		free_env(t_env **env);
void		free_var(t_var **var);
void		free_nonessential(t_shell *store);
void		free_all(t_shell *store);

#endif
