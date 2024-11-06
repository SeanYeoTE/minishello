#include "../core/minishell.h"

t_env	*get_env_loc(t_env *env_list, char *arg)
{
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
		if ((ft_strncmp(env_list->var, arg, count) == 0) && (ft_strncmp(env_list->var, arg, count + 1) == 0))
				return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

t_var	*get_var_loc(char *arg, t_var *var_list, t_env *current_env)
{
	int	count;
	char	*var_data;
	
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

static void print_export(char **str_arr, int env_len)
{
	int i = 0;
	sort_env(str_arr, env_len);
	while (str_arr[i] != NULL)
		printf("declare -x %s\n", str_arr[i++]);
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
	str_array[i] = NULL;
	print_export(str_array, env_len);
	free(str_array);
}

	//check for alpha, '_', 2
	//if -, invalid, 2
	//if other things, invalid identifier, 1
static int	check_arg(char *arg)
{
	int	i;

	i = 0;
	if (!ft_isalpha(arg[i]) || arg[i] != '_')
	{
		if (arg[i] == '-')
			return (2);
		if (ft_isdigit(arg[i]) || arg[i] == '=')
			return (1);
		i++;
	}
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

static void	print_error_msg(char *arg, int ret_value)
{
	if (ret_value == 1)
	{
		ft_putstr_fd("export: '", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
	}
	if (ret_value == 2)
	{
		ft_putstr_fd("export: ", 2);
		write (2, arg, 2);
		ft_putstr_fd(": invalid option\n", 2);
	}
}

static void	set_export(t_shell *store, char *arg)
{
	t_env	*current_env;
	t_var	*current_var;

	current_env = store->env;
	current_var = NULL;
	if (store->var)
		current_var = get_var_loc(arg, store->var, current_env);
	if (is_in_env(current_env, arg))
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
}

int	export_handler(t_shell *store)
{
	char	*arg;
	int		ret_value;

	ret_value = 0;
	if (store->cmd_head->command->next == NULL)
	{
		handle_no_arg(store->env);
		return (0);
	}
	arg = store->cmd_head->command->next->data;
	ret_value = check_arg(arg);
	print_error_msg(arg, ret_value);
	if (ret_value == 0 && ft_strchr(arg, '='))
		set_export(store, arg);
	return (ret_value);
}