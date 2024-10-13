#include "minishell.h"

t_env	*get_env_loc(t_env *env_list, char *arg)
{
	char	*env_data;
	int	count;
	int	i;

	count = -1;
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

static int	get_env_len(t_env *envp)
{
	int	i;
	t_env	*current_env;

	i = 0;
	current_env = envp;
	if (!envp)
		return (i);
	while (current_env)
	{
		i++;
		current_env = current_env->next;
	}
	return (i);
}

static void	sort_env(char **str_array, int env_len)
{
	char	*tmp;
	int		i;
	int		swapped;

	i = 0;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (i < env_len - 1)
		{
			if ((ft_strcmp(str_array[i], str_array[i + 1])) > 0)
			{
				tmp = str_array[i];
				str_array[i] = str_array[i + 1];
				str_array[i + 1] = tmp;
				swapped = 1;
			}
			i++;
		}
		env_len--;
	}
}

static void	handle_no_arg(t_env *envp)
{
	int		env_len;
	int		i;
	char	**str_array;
	t_env	*current_env;
	
	env_len = get_env_len(envp);
	i = 0;
	current_env = envp;
	str_array = malloc((env_len + 1) * sizeof(char *));
	if (str_array == NULL)
		return ;
	while (current_env)
	{
		str_array[i] = current_env->var;
		i++;
		current_env = current_env->next;
	}
	str_array[i] = "\0";
	sort_env(str_array, env_len);
	while (*str_array != NULL)
	{
		printf("declare -x %s\n", *str_array);
		str_array++;
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
	{
		handle_no_arg(store->env);
		return (0);
	}
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