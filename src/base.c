/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/09/29 16:30:10 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prompter(t_shell *store, t_env *env_head, t_var *var_head)
{
	char 	cwd[1024];
	char	*prompt;
	
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	init_var(store, env_head, var_head, store->envp);
	getcwd(cwd, sizeof(cwd));
	prompt = form_prompt(cwd);
	store->input = readline(prompt);
	if (store->input == NULL)
		exit(EXIT_SUCCESS);
	if (store->input[0] == '\0')
	{
		free_nonessential(store);
		return (prompter(store, env_head, var_head));
	}
	add_history(store->input);
	if (!check_quotes(store->input))
		return (print_error("syntax error", NULL));
	pre_execution(store, NULL);
	return (EXIT_SUCCESS);
}

int	pre_execution(t_shell *store, char *input)
{
	store->input = input_spacer(store->input);
	if (ft_strchr(store->input, '$') != NULL)
		store->input = expansions(store->input);
	full_lexer(store->input, store, 0);
	remove_quote(store->head);
	parser(store);
	return (EXIT_SUCCESS);
}

int		parser(t_shell* store)
{
	t_env	*env_head;
	t_var	*var_head;

	if (store->head)
	{
		if (pipe_counter(store->head) == 0)
			single_function(store, store->head, store->tail);
		else if (pipe_counter(store->head) > 0)
			multiple_function(store, pipe_counter(store->head));
	}
	free_nonessential(store);
	env_head = store->env;
	var_head = store->var;
	return (prompter(store, env_head, var_head));
}

int	multiple_function(t_shell *store, int count)
{
	t_node	*front;
	t_node 	*back;
	t_node	*temp;
	bool	create;
	
	front = store->head;
	back = store->head;
	create = true;
	store->pid = ft_calloc(sizeof(int), count + 2);
	while (back->next)
	{
		if (ft_strcmp(back->data, "|") == 0)
		{
			temp = back->next;
			create_cmd(store, front, back->prev, create);
			create = false;
			front = temp;
			back = temp;
		}
		else
			back = back->next;
	}
	create_cmd(store, front, back, create);
	multi_executor(store, count_cmds(store) - 1);
	revert_nodes(store);
	return (0);
}
