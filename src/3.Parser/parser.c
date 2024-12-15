/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:32:14 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 12:14:22 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Parses the preprocessed input and routes to appropriate execution
 * 	handlers
 * @param store Main shell data structure containing command information
 * @param should_continue Pointer to boolean indicating if shell loop should
 * 	continue
 * @return EXIT_SUCCESS after successful parsing and execution
 * @details Analyzes command structure to determine execution path:
 * - For single commands without pipes, routes to single_function
 * - For commands with pipes, routes to multiple_function
 * - For empty input, frees non-essential resources and continues
 */
int	parser(t_shell *store, bool *should_continue)
{
	if (store->head)
	{
		if (pipe_counter(store->head) == 0)
			single_function(store, store->head, store->tail);
		else if (pipe_counter(store->head) > 0)
			multiple_function(store);
	}
	else if (store->input[0] == '\0')
	{
		free_nonessential(store);
		*should_continue = true;
		return (EXIT_SUCCESS);
	}
	free_nonessential(store);
	*should_continue = true;
	return (EXIT_SUCCESS);
}

/**
 * @brief Creates a new command structure from a sequence of nodes
 * @param store Main shell data structure
 * @param start First node in command sequence
 * @param end Last node in command sequence
 * @param create Boolean flag indicating if this is first command
 * @return 0 on success, 1 on failure
 * @details Initializes new command structure using provided nodes
 * Returns error if start node is NULL or command initialization fails
 */
int	create_cmd(t_shell *store, t_node *start, t_node *end, bool create)
{
	t_cmd	*new;

	if (!start)
		return (1);
	new = init_cmd(store, start, end, create);
	if (!new)
		return (1);
	return (0);
}
