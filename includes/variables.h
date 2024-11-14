/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variables.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:19:10 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/14 13:06:31 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARIABLES_H
# define VARIABLES_H

# include "minishell.h"

// expansions.c
char		*expansions(t_shell *store, char *input);

// expansions_utils.c
char		*extract_var_name(const char *input, int start, int *end);
void		update_quote_state(char c, bool *in_single_quotes,
				bool *in_double_quotes);
bool		should_expand(bool in_single_quotes);
char		*remove_dollar_quotes(char *input, int start);
int			handle_export(t_shell *store, char *current_arg);

// expansions_replace.c
char		*replace_var(char *input, int start, int end, const char *value);
char		*replace_exit_status(t_shell *store, char *input, int start);

// expansions_state.c
void		handle_dollar_quotes(char **input, int *i,
				t_quote_state *quote_state);
void		handle_exit_status(t_shell *store, char **input, int *i);
void		handle_variable(char **input, int *i, t_shell *store);
void		init_expansion_state(bool *in_single_quotes,
				bool *in_double_quotes);

// remove_quote.c
void		remove_quote(t_node *token);

// var_handler.c
t_var		*create_var_node(char *var, char *data);
bool		same_env(char *src, t_shell *store);
bool		same_var(char *src, t_shell *store);
t_var		*split_var(char *src);
int			var_handler(t_node *args, t_shell *store);

// var_utils.c
int			name_counter(char *src);
char		**ft_split_var(char const *str, char c);
int			set_var(char *src, t_shell *store);

#endif
