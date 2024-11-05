/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/05 23:34:06 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prompter(t_shell *store, t_env *env_head, t_var *var_head)
{
	char 	cwd[1024];
	char	*prompt;
	
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	init_var(store, env_head, var_head);
	
	// print_argv(store->envp);
	getcwd(cwd, sizeof(cwd));
	prompt = form_prompt(cwd);
	store->input = readline(prompt);
	free(prompt);
	if (store->input == NULL)
	{
		free_all(store);
		exit(EXIT_SUCCESS);
	}
	if (store->input[0] == '\0')
	{
		free_nonessential(store);
		return (prompter(store, env_head, var_head));
	}
	add_history(store->input);
	if (check_error(store->input))
	{
		print_erroronly("syntax error", store->input);
		free_nonessential(store);
		return (prompter(store, env_head, var_head));
	}
	pre_execution(store);
	free_all(store);
	return (EXIT_SUCCESS);
}

int	pre_execution(t_shell *store)
{
	store->input = input_spacer(store->input);
	if (ft_strchr(store->input, '$') != NULL)
	{
		store->expanded = true;
		store->input = expansions(store->input);
	}
	// printf("input: %s\n", store->input);
	full_lexer(store->input, store, 0);
	// print_stack(&store->head);
	remove_quote(store->head);
	// print_stack(&store->head);
	parser(store);
	return (EXIT_SUCCESS);
}

int		parser(t_shell* store)
{
	t_env	*env_head;
	t_var	*var_head;

	env_head = store->env;
	var_head = store->var;
	t_exit_status = 0;
	if (store->head)
	{
		if (pipe_counter(store->head) == 0)
			single_function(store, store->head, store->tail);
		else if (pipe_counter(store->head) > 0)
			multiple_function(store, pipe_counter(store->head));
		env_head = store->env;
		var_head = store->var;
	}
	
	else if (store->input[0] == '\0')
	{
		free_nonessential(store);
		return (prompter(store, env_head, var_head));
	}
	free_nonessential(store);
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
	while (back->next)
	{
		if (ft_strcmp(back->data, "|") == 0)
		{
			temp = back->next;
			create_cmd(store, front, back->prev, create);
			create = false;
			free(back->data);
			free(back);
			if (temp)
				temp->prev = NULL;
			front = temp;
			back = temp;
		}
		else
			back = back->next;
	}
	create_cmd(store, front, back, create);
	multi_executor(store, count_cmds(store) - 1);
	return (0);
}
