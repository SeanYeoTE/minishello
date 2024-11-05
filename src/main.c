/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/05 23:55:35 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		t_exit_status;

static int	count_env(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

static char	**init_environment(char **envp)
{
	char	**new_env;
	int		i;
	int		env_count;

	env_count = count_env(envp);
	new_env = (char **)malloc(sizeof(char *) * (env_count + 1));
	if (!new_env)
	{
		perror("Failed to allocate memory for environment");
		exit(1);
	}
	i = 0;
	while (i < env_count)
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			perror("Failed to copy environment variable");
			exit(1);
		}
		i++;
	}
	new_env[env_count] = NULL;
	return (new_env);
}

char *	cgetenv(char *var, t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		// printf("current->var: %s\n", current->var);
		if (ft_strncmp(var, current->var, ft_strlen(var)) == 0)
			return (current->var + ft_strlen(var) + 1);
		current = current->next;
	}
	return (NULL);
}

char*	cprintvar(t_var *var)
{
	t_var	*current;

	current = var;
	while (current)
	{
		// printf("current->var: %s\n", current->var);
		printf("name: %s\n", current->name);
		printf("data: %s\n", current->data);
		printf("hidden: %s\n", current->hidden);
		current = current->next;
	}
	return (NULL);
}

char **ccreatearray(t_env *env)
{
	t_env	*current;
	char	**new_env;
	int		i;

	current = env;
	i = 0;
	while (current)
	{
		i++;
		current = current->next;
	}
	new_env = (char **)malloc(sizeof(char *) * (i + 1));
	if (!new_env)
	{
		perror("Failed to allocate memory for environment");
		exit(1);
	}
	i = 0;
	current = env;
	while (current)
	{
		new_env[i] = current->var;
		i++;
		current = current->next;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	init_var(t_shell *store, t_env *env_head, t_var *var_head)
{		
	store->input_reset = dup(STDIN_FILENO);
	store->output_reset = dup(STDOUT_FILENO);
	store->head = NULL;
	store->tail = NULL;
	store->fd_in = STDIN_FILENO;
	store->quotes = false;
	store->expanded = false;
	store->cmd_head = NULL;
	store->cmd_tail = NULL;
	
	store->env = env_head;
	store->var = var_head;

	store->path = ft_strdup(cgetenv("PATH", env_head));
	store->paths = ft_split(store->path, ':');
	store->envp = ccreatearray(env_head);

	// cprintvar(store->var);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	store;
	t_env	*env_head;
	char	**envp1;
	
	if (argc != 1 || argv[1])
	{
		perror("run without args");
		return (0);
	}
	env_head = env_init(&store, envp);
	t_exit_status = 0;
	return (prompter(&store, env_head, NULL));
}
