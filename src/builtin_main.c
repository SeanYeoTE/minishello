/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:40:05 by seayeo            #+#    #+#             */
/*   Updated: 2024/07/21 17:35:45 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//builtin main
int	builtin_main(t_shell *store, t_node *current, t_node *end)
{
	t_node	*ret;
	int		exit_status;
	(void)store;
	
	if (ft_strcmp(current->data, "cd") == 0)
		exit_status = cd_handler(current);
	else if (!ft_strcmp(current->data, "echo"))
		exit_status = echo_handler(current, end);
	else if (!ft_strcmp(current->data, "pwd"))
		exit_status = pwd_handler(current);
	else if (!ft_strcmp(current->data, "env"))
		env_handler(store);
	else
		exit_status = var_handler(current->data, store->env);
	return (exit_status);
}

// cd handler
int	cd_handler(t_node *current)
{
	char	*home;

	if (current->next == NULL || ft_strcmp(current->next->data, "~") == 0)
	{
		home = getenv("HOME");
		if (home == NULL || chdir(home) != 0)
		{
			perror("no home");
			return (1);
		}
	}
	else if (chdir(current->next->data) != 0)
		perror(current->data);
	return (0);
}

// echo handler
int	echo_handler(t_node *current, t_node *end)
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
	return (0);
}

//pwd handler
int	pwd_handler(t_node *current)
{
	char	*cwd;
	size_t	cwd_buf;

	cwd_buf = 1000;
	cwd = malloc (cwd_buf * sizeof(char));
	if (cwd == NULL)
	{
		perror("Invalid Memory Allocation\n");
		return (1);
	}
	if (getcwd(cwd, cwd_buf) != NULL)
		printf("%s\n", cwd);
	else
	{
		perror("PWD Error\n");
		free(cwd);
		return (1);
	}
	return (0);
}

//env handler
t_env	*create_env_node(char *var, char *data, bool flag)
{
		t_env	*new_node;
		char	*vari;//to fix this, env->name cannot be NULL

		vari = "abc";
		new_node = ft_calloc(1, sizeof(t_env));
		if (!new_node)
			perror("Failed to allocate memory");
		if (!flag)
		{
			new_node->var = ft_strdup(var);
			new_node->next = NULL;
			new_node->name = vari;
			new_node->data = NULL;
			return (new_node);
		}
		else
		{
			new_node->var = NULL;
			new_node->name = ft_strdup(var);
			new_node->data = ft_strdup(data);
			return (new_node);
		}
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

		new_node = create_env_node(envp[i], NULL, false);
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

//export handler
int	var_handler(char *src, t_env *env)
{
	int	i;
	int	j;
	char	name[1024];
	char	value[1024];
	t_env	*current;

	i = 0;
	j = 0;
	current = env;
	//find the = operator
	//check if name is in the system, if yes, replace value
	//label the first portion as variable name
	//label the second portion as data
	//link the linked list
	while ((src[i] != '=') && src[i])
	{
		name[i] = src[i];
		i++;
	}
	name[i] = '\0';
	while (src[i])
	{
		i++;
		value[j] = src[i];
		j++;
	}
	while (current && current->next)
	{
		if (ft_strcmp(name, current->name) == 0)
		{
			free(current->data);
			current->data = NULL;
			current->data = ft_strdup(value);
		}
		else
			current = current->next;
	}
	current->next = create_env_node(name, value, true);
	current->next->next = NULL;
	return (0);
}

//during export search the entire 
