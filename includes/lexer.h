/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:15:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/26 19:33:19 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

// lexer.c
bool		is_space(char c);
int			full_lexer(char *str, t_shell *store, int index);
int			lexer(t_shell *store);

// checks.c
int			redir_checker(t_node *cmd);
int			check_builtin(t_node *loop);
int			is_operator(char c);
int			is_double_operator(const char *input, int i);

// checks2.c
int			check_quotes(char *line);
int			check_error(char *input);

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

// t_node_utils.c
int			init_node(char *value, t_node **head);
t_node		*get_last(t_node *last);
t_node		*get_node(t_node *ret, int num);



#endif
