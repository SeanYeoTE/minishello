/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:38:47 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 11:50:05 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

// parser.c
int			parser(t_shell *store, bool *should_continue);
int			create_cmd(t_shell *store, t_node *start, t_node *end, bool create);

// t_cmd_utils.c
t_cmd		*init_cmd(t_shell *store, t_node *start, t_node *end, bool create);
void		setup_cmd_links(t_shell *store, t_cmd *cmd, bool create);
void		detach_redir(t_cmd *new);
void		init_cmd_fds(t_cmd *cmd);

// t_cmd_utils_extra.c
void		set_parent(t_node *node, t_cmd *cmd);
void		remove_nodes(t_node **start, t_node *redir, t_node *file);
void		add_to_redir(t_node **redir, t_node *new_redir, t_node *new_file);
t_cmd		*get_last_cmd(t_cmd *cmd);
int			count_cmds(t_shell *store);

#endif 