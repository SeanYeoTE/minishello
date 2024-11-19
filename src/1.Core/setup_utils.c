/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 14:04:27 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/19 20:51:25 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initializes the shell's variables and state
 * @param store Main shell data structure
 * @param state Shell state structure to copy from
 * @details Sets up initial shell state including:
 *          - File descriptors for I/O
 *          - Command and token linked lists
 *          - Environment variables
 *          - PATH variable and its components
 */
void	init_var(t_shell *store, t_shell_state *state)
{
	store->exit_status = state->exit_status;
	store->input_reset = dup(STDIN_FILENO);
	store->output_reset = dup(STDOUT_FILENO);
	store->head = NULL;
	store->tail = NULL;
	store->fd_in = STDIN_FILENO;
	store->quotes = false;
	store->expanded = false;
	store->cmd_head = NULL;
	store->cmd_tail = NULL;
	store->env = state->env;
	store->var = state->var;
	store->path = ft_strdup(ft_getenv("PATH", store->env));
	store->paths = ft_split(store->path, ':');
	store->envp = ft_createarray(store->env);
}

/**
 * @brief Updates the shell state structure with current values
 * @param store Main shell data structure
 * @param state Shell state structure to update
 * @details Copies current environment, variables and exit status
 *          from the store into the state structure
 */
void	update_shell_state(t_shell *store, t_shell_state *state)
{
	state->env = store->env;
	state->var = store->var;
	state->exit_status = store->exit_status;
}

/**
 * @brief Initializes the shell prompt string
 * @param prompt Pointer to the prompt string to initialize
 * @details Sets the prompt to the current working directory
 */
void	prompter_init(char **prompt)
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
 * @return 1 if input is valid and ready for processing, 
 * 0 if input should be skipped
 * @details Reads user input, handles EOF, empty input, and basic syntax checking
 */
int	prompter_input(t_shell *store, char *prompt)
{
	store->input = readline(prompt);
	free(prompt);
	if (g_sig == 1)
	{
		store->exit_status = 130;
		g_sig = 0;
	}
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
