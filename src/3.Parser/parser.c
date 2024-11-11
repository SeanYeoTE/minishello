/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:32:14 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/11 15:36:35 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Parses the preprocessed input and routes to appropriate execution 
 * handlers
 * @param store Main shell data structure
 * @param should_continue Pointer to boolean indicating if the loop should 
 * continue
 * @return EXIT_SUCCESS after successful parsing and execution
 * @details Determines if command requires pipe handling and routes to 
 * appropriate handler
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