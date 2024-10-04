/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/04 15:32:11 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prompter(t_shell *store, t_env *env_head, t_var *var_head, char **envp)
{
	char 	cwd[1024];
	char	*prompt;
	
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	init_var(store, env_head, var_head, envp);
	getcwd(cwd, sizeof(cwd));
	prompt = form_prompt(cwd);
	store->input = readline(prompt);
	free(prompt);
	if (store->input == NULL)
		exit(EXIT_SUCCESS);
	if (store->input[0] == '\0')
	{
		// need to rewrite otherwise point to null
		free_nonessential(store);
		return (prompter(store, env_head, var_head, envp));
	}
	add_history(store->input);
	if (!check_quotes(store->input))
		return (print_error("syntax error", NULL));
	pre_execution(store);
	return (EXIT_SUCCESS);
}

int	pre_execution(t_shell *store)
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
	char 	**envp;

	if (store->head)
	{
		if (pipe_counter(store->head) == 0)
			single_function(store, store->head, store->tail);
		else if (pipe_counter(store->head) > 0)
			multiple_function(store, pipe_counter(store->head));
	}
	envp = store->envp;
	env_head = store->env;
	var_head = store->var;
	free_nonessential(store);
	return (prompter(store, env_head, var_head, envp));
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
