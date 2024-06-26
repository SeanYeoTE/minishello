/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:40:05 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/26 20:46:03 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//builtin main
t_node	*builtin_main(t_shell *store, t_node *current, t_node *end)
{
	t_node	*ret;

	(void)store;
	
	if (ft_strcmp(current->data, "cd") == 0)
		ret = cd_handler(current);
	else if (!ft_strcmp(current->data, "echo"))
		ret = echo_handler(current, end);
	else if (!ft_strcmp(current->data, "pwd"))
		ret = pwd_handler(current);
	return (ret);
}

// cd handler
t_node	*cd_handler(t_node *current)
{
	char	*home;

	if (current->next == NULL || ft_strcmp(current->next->data, "~") == 0)
	{
		home = getenv("HOME");
		if (home == NULL || chdir(home) != 0)
			perror("no home");
	}
	else if (chdir(current->next->data) != 0)
		perror(current->data);
	return (current);
}

// echo handler
t_node	*echo_handler(t_node *current, t_node *end)
{
	int	option;	

	option = 0;
	current = current->next;
	if (ft_strncmp(current->data, "-n", 2) == 0)
		option = 1;
	if (option == 1)
		current = current->next;
	while (current != end)
	{
		printf("%s", current->data);
		printf(" ");
		current = current->next;  
	}
	if (option == 0)
		printf("\n");
	return (current);
}

//pwd handler
t_node	*pwd_handler(t_node *current)
{
	char	*cwd;
	size_t	cwd_buf;

	cwd_buf = 1000;
	cwd = malloc (cwd_buf * sizeof(char));
	if (cwd == NULL)
		perror("Invalid Memory Allocation\n");
	if (getcwd(cwd, cwd_buf) != NULL)
		printf("%s\n", cwd);
	else
	{
		perror("PWD Error\n");
		free (cwd);
	}
	return (current->next);
}
//env handler
t_env	*create_env_node(char *env_var)
{
		t_env	*new_node;

		new_node = ft_calloc(1, sizeof(t_env));
		if (!new_node)
			perror("Failed to allocate memory");
		new_node->var = ft_strdup(env_var);
		new_node->next = NULL;
		return (new_node);
}

void	env_init(t_shell *store, char **envp)
{
	t_env	*current;
	int	i;

	current = NULL;
	store->env = NULL;
	i = 0;
	while (envp[i])
	{
		t_env	*new_node;

		new_node = create_env_node(envp[i]);
		if (!store->env)
			store->env = new_node;
		else
			current->next = new_node;
		current = new_node;
		i++;
	}
}

void	env_handler(t_shell *store)
{
	t_env	*current;

	current = store->env;
	while (current)
	{
		printf("%s\n", current->var);
		current = current->next;
	}
}