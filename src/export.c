#include "minishell.h"

t_env	*get_env_loc(t_env *env_list, char *arg)
{
	char	*env_data;
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (arg[i] && arg[i] != '=')
	{
		count++;
		i++;
	}
	while (env_list)
	{
		env_data = env_list->var;
		if ((ft_strncmp(env_list->var, arg, count) == 0) && (ft_strncmp(env_list->var, arg, count + 1) == 0))
				return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

t_var	*get_var_loc(char *arg, t_var *var_list, t_env *current_env)
{
	int	i;
	int	count;
	char	*var_data;
	
	i = 0;
	count = 0;
	if (!var_list)
		return (NULL);
	count = ft_strlen(arg);
	while (var_list)
	{
		var_data = var_list->name;
		if (ft_strncmp(var_data, arg, count) == 0 && ft_strcmp(arg, current_env->var) != 0)
			return (var_list);
		var_list = var_list->next;
	}
	return (NULL);
}

static void	handle_no_arg(char **envp)
{
	char **arg;

	arg = envp;
	while (*arg != NULL)
	{
		//need to print
		//need to sort env

		printf("declare -x %s\n", *arg);
		arg++;
	}
}

int	export_handler(t_shell *store)
{
	t_env	*new_env;
	t_env	*current_env;
	t_var	*current_var;
	char	*arg;

	current_env = store->env;
	current_var = NULL;
	if (store->cmd_head->command->next == NULL)
		handle_no_arg(store->envp);
	else
		arg = store->cmd_head->command->next->data;
	if (store->var)
		current_var = get_var_loc(arg, store->var, current_env);
	if (is_in_env(current_env, arg, store))
		{
			current_env = get_env_loc(store->env, arg);
			free (current_env->var);
			if (current_var)
				current_env->var = ft_strdup(current_var->hidden);
			else if (!current_var && ft_strchr(arg, '='))
				current_env->var = ft_strdup(arg);
		}
	else
	{
		current_env = get_last_env(current_env);
		if (current_var)
			current_env->next = create_env_node(current_var->hidden);			
		else
			current_env->next = create_env_node(arg);
	}
	return (0);
}