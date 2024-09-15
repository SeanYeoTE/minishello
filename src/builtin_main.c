/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:40:05 by seayeo            #+#    #+#             */
/*   Updated: 2024/09/15 16:21:40 by mchua            ###   ########.fr       */
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
	else if (!ft_strcmp(current->data, "export"))
		exit_status = export_handler(store);
	else
		exit_status = var_handler(current->data, store);
	//exit(exit_status);
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
	int		option;
	bool	printed;

	option = 0;
	current = current->next;
	if (ft_strncmp(current->data, "-n", 2) == 0)
		option = 1;
	if (option == 1)
		current = current->next;
	while (current != end)
	{
		printed = true;
		printf("%s", current->data);
		if (current->next)
			printf(" ");
		current = current->next;  
	}
	if (option == 0 || printed == false)
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
t_env	*create_env_node(char *var)
{
	t_env	*new_env;

	new_env = ft_calloc(1, sizeof(t_env));
	if (!new_env)
		perror("Failed to allocate memory");
	new_env->var = ft_strdup(var);
	new_env->next = NULL;
	return (new_env);
}

t_env	*env_init(t_shell *store, char **envp)
{
	t_env	*current;
	t_env	*head;
	t_env	*new_node;
	int	i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = create_env_node(envp[i]);

		if (head == NULL)
		{
			head = new_node;
			current = head;
		}
		else
			current = current->next;
		current->next = new_node;
		i++;
	}
	return (head);
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
t_var	*create_var_node(char *var, char *data)
{
	t_var	*new_var;

	new_var = ft_calloc(1, sizeof(t_var));
	if (!new_var)
		perror("Failed to allocate memory");
	new_var->name = ft_strdup(var);
	new_var->data = ft_strdup(data);
	new_var->hidden = NULL;
	new_var->next = NULL;
	return (new_var);
}

t_var	*split_var(char *src, t_var *var)
{
	int	i;
	int	j;
	t_var	*new_var;
	char	name[1024];
	char	value[1024];


	i = 0;
	j = 0;
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
	// new_var = var_init(src, name, value, var);
	

	new_var = create_var_node(name, value);
	new_var->hidden = ft_strdup(src);
	return (new_var);
}

// t_var	*var_init(char *src, char *name, char *value, t_var *var)
// {
// 	t_var	*new_var;
// 	t_var	*head;

// 	// current = var;
// 	// while (current)
// 	// {
// 	// 	if (current->name != NULL)
// 	// 	{
// 	// 		if (ft_strcmp(name, current->name) == 0)
// 	// 		{
// 	// 			free(current->data);
// 	// 			current->data = NULL;
// 	// 			current->data = ft_strdup(value);
// 	// 			return (0);
// 	// 		}
// 	// 	}
// 	// 	current = current->next;
// 	// }
// 	head = var;
// 	new_var = create_var_node(name, value);
// 	new_var->hidden = ft_strdup(src);
	
// 	if (!var)
// 	{
// 		var = new_var;
// 		head = var;
// 	}
// 	// while (var->next)
// 	// {
// 	// 	var = var->next;
// 	// 	if (var->next == NULL)
// 	// 		var->next = new_var;
// 	// }



// 	else
// 	{
// 		while (var)
// 		{
// 			if (var->next == NULL)
// 			{
// 				var->next = new_var;
// 				break ;
// 			}
// 			var = var->next;
// 		}
// 	}
// 	// current = current->next;
// 	// current = new_var;
// 	var = head;
// 	return (var);
// }

static int print_var(t_var *var)
{
	t_var	*start;
	int		count;

	count = 0;
	start = var;
	while (start)
	{
		printf("Var: %s\n", start->hidden);
		printf("***************\n");
		start = start->next;
		count++;
	}
	printf("Total Nodes: %d\n", count);
	return (0);
}

static int	name_counter(char *src)
{
	int	n;
	bool checker;

	checker = false;

	n = 0;
	while (*src != '\0')
	{
		if (*src != '=')
			n++;
		else
		{
			checker = true;
			break ;
		}
		src++;
	}
	if (checker)
		return (n);
	else
		return (0);
}

static bool	same_env(char *src, t_shell *store)
{
	t_env	*current;
	int	count;

	count = name_counter(src);
	current = store->env;
	while (current)
	{
		if (ft_strcmp(src, current->var) == 0)
			return (false);
		else if (ft_strncmp(src, current->var, count) == 0 && ft_strcmp(src, current->var) != 0)
		{
			free (current->var);
			current->var = ft_strdup(src);
			return (false);
		}
		current = current->next;
	}
	return (true);
}

static bool	same_var(char *src, t_shell *store)
{
	t_var	*current;
	int	count;

	count = name_counter(src);
	current = store->var;
	if (!current)
		return (false);
	else
	{
		while (current)
		{
			if (ft_strcmp(src, current->hidden) == 0)
				return true;
			else if (ft_strncmp(src, current->hidden, count) == 0 && ft_strcmp(src, current->hidden) != 0)
			{
				free (current->hidden);
				current->hidden = ft_strdup(src);
				return true;
			}
			current = current->next;
		}
	}
	return false;
}

int	var_handler(char *src, t_shell *store)
{
	t_var	*new_var;
	t_var	*current;

	new_var = NULL;
	if (!same_var(src, store) || !same_env(src, store))
		new_var = split_var(src, store->var);
	if (store->var == NULL)
		store->var = new_var;
	else
	{
		current = store->var;
		while (current)
		{
			if (current->next == NULL && new_var != NULL)
			{
				current->next = new_var;
				break ;
			}
			current = current->next;
		}
	}
	return (0);
}

// during export search the entire
static int	args_key_counter(char *src)
{
	int	i;

	i = 0;
	while (*src != '\0')
	{
		src++;
		i++;
	}
	return (i);
}

static	t_env	*get_env_loc(t_env *env_list, char *arg)
{
	char	*env_data;
	while (env_list)
	{
		env_data = env_list->var;
		while (*arg)
		{
			if (*env_data == *arg)
			{
				arg++;
				env_data++;
			}
			else
				break ;
		}
		env_list = env_list->next;
	}
	return (env_list);
}

static t_var	*get_var_loc(char *arg, t_var *var_list)
{
	int	count;
	char	*var_data;
	if (!var_list)
		return (NULL);
	count = ft_strlen(arg);
	while (var_list)
	{
		var_data = var_list->name;
		if (ft_strncmp(var_data, arg, count) == 0 || ft_strchr(arg, '=') != NULL)
			return (var_list);
		var_list = var_list->next;
	}
	return (NULL);
}

static bool	is_in_env(t_env *env_list, char *arg, t_shell *store)
{
	int	count;
	t_env	*current_env;
	count = args_key_counter(arg);

	current_env = get_env_loc(env_list, arg);
	if (!current_env)
		return (false);
	return (true);
}

static t_env	*get_last_env(t_env *current_env)
{
	t_env	*last_env;

	last_env = current_env;
	while (last_env->next)
		last_env = last_env->next;
	return (last_env);
}

int	export_handler(t_shell *store)
{
	t_env	*new_env;
	t_env	*current_env;
	t_var	*current_var;
	char	*arg;

	current_env = store->env;
	arg = store->cmd_head->command->next->data;
	current_var = get_var_loc(arg, store->var);
	if (is_in_env(current_env, arg, store))
		{
			current_env = get_env_loc(store->env, arg);
			free (current_env->var);
			if (current_var)
				current_env->var = ft_strdup(store->var->hidden);
			else
				current_env->var = ft_strdup(arg);
		}
	else
	{
		current_env = get_last_env(current_env);
		if (current_var)
			current_env->next = create_env_node(store->var->hidden);			
		else
			current_env->next = create_env_node(arg);
	}
	return (0);
}