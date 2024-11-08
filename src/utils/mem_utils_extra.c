/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils_extra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:58:10 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/08 15:00:16 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

void	free_env(t_env **env)
{
	t_env	*tmp;
	t_env	*current;

	if (!env)
		return ;
	current = *env;
	while (current)
	{
		tmp = current->next;
		free(current->var);
		free(current);
		current = tmp;
	}
	*env = NULL;
}

void	free_var(t_var **var)
{
	t_var	*tmp;
	t_var	*current;

	if (!var)
		return ;
	current = *var;
	while (current)
	{
		tmp = current->next;
		free(current->data);
		free(current->name);
		free(current->hidden);
		free(current);
		current = tmp;
	}
	*var = NULL;
}

void	free_nonessential(t_shell *store)
{
	check_open_fds(256);
	if (store->paths)
		freechararray(store->paths);
	if (store->path)
		free(store->path);
	if (store->envp)
		free(store->envp);
	if (store->input)
		free(store->input);
	if (store->cmd_head)
		free_cmd(&(store->cmd_head));
	else if (store->head)
		free_stack(&(store->head));
}

void	free_all(t_shell *store)
{
	check_open_fds(256);
	if (store->paths)
		freechararray(store->paths);
	if (store->path)
		free(store->path);
	if (store->input)
		free(store->input);
	if (store->envp)
		free(store->envp);
	if (store->cmd_head)
		free_cmd(&(store->cmd_head));
	else if (store->head)
		free_stack(&(store->head));
	if (store->env)
		free_env(&(store->env));
	if (store->var)
		free_var(&(store->var));
}
