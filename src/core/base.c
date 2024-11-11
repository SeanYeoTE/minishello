/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 12:09:20 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

int	prompter(t_shell *store, t_env *env_head, t_var *var_head, int exit_status);
int	pre_execution(t_shell *store);
int	parser(t_shell *store);


static void	prompter_init(char **prompt)
{
	char	cwd[1024];

	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
	getcwd(cwd, sizeof(cwd));
	*prompt = form_prompt(cwd);
}

/**
 * @brief Handles reading and initial processing of user input
 * @param store Main shell data structure
 * @param prompt The prompt string to display
 * @return 1 if input is valid and ready for processing, 0 if input should be skipped
 * @details Reads user input, handles EOF, empty input, and basic syntax checking
 */
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

/**
 * @brief Main prompt loop that manages the shell's read-eval-print cycle
 * @param store Main shell data structure
 * @param env_head Environment variables list
 * @param var_head Shell variables list
 * @param exit_status Last command's exit status
 * @return EXIT_SUCCESS after successful command execution
 * @details Coordinates the shell's main operation cycle including input, processing, and execution
 */
int	prompter(t_shell *store, t_env *env_head, t_var *var_head, int exit_status)
{
	char	*prompt;

	prompter_init(&prompt);
	init_var(store, env_head, var_head, exit_status);
	if (!prompter_input(store, prompt))
	{
		free_nonessential(store);
		return (prompter(store, env_head, var_head, exit_status));
	}
	pre_execution(store);
	free_all(store);
	return (EXIT_SUCCESS);
}

/**
 * @brief Prepares the input string for execution by handling various preprocessing steps
 * @param store Main shell data structure
 * @return EXIT_SUCCESS after successful preprocessing
 * @details Handles input spacing, variable expansions, lexical analysis, and quote removal
 */
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

/**
 * @brief Parses the preprocessed input and routes to appropriate execution handlers
 * @param store Main shell data structure
 * @return Returns to prompter with updated shell state
 * @details Determines if command requires pipe handling and routes to appropriate handler
 */
int	parser(t_shell *store)
{
	t_env	*env_head;
	t_var	*var_head;
	int 	exit_status;

	env_head = store->env;
	var_head = store->var;
	exit_status = store->exit_status;
	if (store->head)
	{
		if (pipe_counter(store->head) == 0)
			single_function(store, store->head, store->tail);
		else if (pipe_counter(store->head) > 0)
			multiple_function(store);
		env_head = store->env;
		var_head = store->var;
		exit_status = store->exit_status;
	}
	else if (store->input[0] == '\0')
	{
		free_nonessential(store);
		return (prompter(store, env_head, var_head, exit_status));
	}
	free_nonessential(store);
	return (prompter(store, env_head, var_head, exit_status));
}
