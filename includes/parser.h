/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <seayeo@42.sg>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:38:47 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/20 19:00:31 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

// parser.c
int			parser(t_shell *store, bool *should_continue);
int			create_cmd(t_shell *store, t_node *start, t_node *end, int create);

// t_cmd_utils.c
t_cmd		*init_cmd(t_shell *store, t_node *start, t_node *end, int create);
void		setup_cmd_links(t_shell *store, t_cmd *cmd, int create);
void		detach_redir(t_cmd *new);
void		init_cmd_fds(t_cmd *cmd);

// t_cmd_utils_extra.c
void		set_parent(t_node *node, t_cmd *cmd);
void		remove_nodes(t_node **start, t_node *redir, t_node *file);
void		add_to_redir(t_node **redir, t_node *new_redir, t_node *new_file);
t_cmd		*get_last_cmd(t_cmd *cmd);
int			count_cmds(t_shell *store);

#endif 