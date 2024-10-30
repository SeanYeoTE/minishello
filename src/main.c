/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/30 04:34:02 by seayeo           ###   ########.fr       */
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

void	init_var(t_shell *store, t_env *env_head, t_var *var_head, char **envp)
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
	store->path = getenv("PATH");
	store->paths = ft_split(store->path, ':');
	store->envp = envp;
	store->pid = NULL;
	store->env = env_head;
	store->var = var_head;
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
	envp1 = init_environment(envp);
	return (prompter(&store, env_head, NULL, envp1));
}
