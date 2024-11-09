/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/09 18:23:05 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

int	prompter(t_shell *store, t_env *env_head, t_var *var_head);
int	pre_execution(t_shell *store);
int	parser(t_shell *store);

static void	prompter_init(t_shell *store, t_env *env_head, t_var *var_head,
		char **prompt)
{
	char	cwd[1024];

	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	init_var(store, env_head, var_head);
	getcwd(cwd, sizeof(cwd));
	*prompt = form_prompt(cwd);
}

static int	prompter_input(t_shell *store, char *prompt)
{
	store->input = readline(prompt);
	free(prompt);
	if (store->input == NULL)
	{
		free_all(store);
		exit(EXIT_SUCCESS);
	}
	if (store->input[0] == '\0')
		return (0);
	add_history(store->input);
	if (check_error(store->input))
	{
		print_erroronly("syntax error", store->input);
		return (0);
	}
	return (1);
}

int	prompter(t_shell *store, t_env *env_head, t_var *var_head)
{
	char	*prompt;

	prompter_init(store, env_head, var_head, &prompt);
	if (!prompter_input(store, prompt))
	{
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
		store->input = expansions(store, store->input);
	}
	full_lexer(store->input, store, 0);
	remove_quote(store->head);
	parser(store);
	return (EXIT_SUCCESS);
}

int	parser(t_shell *store)
{
	t_env	*env_head;
	t_var	*var_head;

	env_head = store->env;
	var_head = store->var;
	g_exit_status = 0;
	if (store->head)
	{
		if (pipe_counter(store->head) == 0)
			single_function(store, store->head, store->tail);
		else if (pipe_counter(store->head) > 0)
			multiple_function(store);
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
