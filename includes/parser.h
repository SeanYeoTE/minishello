/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:15:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 14:12:41 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

// checks.c
int			redir_checker(t_node *cmd);
int			check_builtin(t_node *loop);
int			is_operator(char c);
int			is_double_operator(const char *input, int i);

// checks2.c
int			check_quotes(char *line);
int			check_error(char *input);

// parse_detection.c
bool		is_space(char c);
int			full_lexer(char *str, t_shell *store, int index);

// input_utils.c
char		*input_spacer(char *input);

// input_quote.c
bool		within_quote(const char *input, int index);

// scanner.c
int			scanner_comment(char *str, int start, t_shell *store);
int			scanner_quote(char *str, int start, t_shell *store);
int			scanner_operator(char *str, int start, t_shell *store);
int			scanner_space(char *str, int start);
int			scanner_word(char *str, int start, t_shell *store);

// args_init.c
int			init_node(char *value, t_node **head);
t_node		*get_last(t_node *last);
t_node		*get_node(t_node *ret, int num);

// t_cmd_utils.c
t_cmd		*get_last_cmd(t_cmd *cmd);
int			count_cmds(t_shell *store);
t_cmd		*init_cmd(t_shell *store, t_node *start, t_node *end, bool create);

// t_cmd_utils_extra.c
void		set_parent(t_node *node, t_cmd *cmd);
void		remove_nodes(t_node **start, t_node *redir, t_node *file);
void		add_to_redir(t_node **redir, t_node *new_redir, t_node *new_file);
int			create_cmd(t_shell *store, t_node *start, t_node *end, bool create);
void		detach_redir(t_cmd *new);

#endif
