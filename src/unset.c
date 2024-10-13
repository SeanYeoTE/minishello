#include "minishell.h"

static bool	is_valid_var_name(char *arg)
{
	int	i;

	i = 0;
	if (!arg || !ft_isalpha(arg[i] || arg[i] != '_'))
		return (false);
	while (arg[i])
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_' && arg[i] != ';')
			return (false);
		i++;
	}
	return (true);
}
static bool	got_equal(t_env *current_env, char *arg)
{
	if (ft_strchr(arg, '=') != NULL)
		return true;
	return false;
}

static t_env	*get_target_loc(t_env *env_list, char *arg)
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
		if (ft_strncmp(env_list->var, arg, count) == 0)
				return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

static bool	perform_unset(char *arg, t_shell *store)
{
	t_env	*previous_node;//node before target unset arg
	t_env	*next_node;//node after target unset arg
	t_env	*target_env;

	target_env = get_target_loc(store->env, arg);
	if (!target_env)
		return (false);
	previous_node = store->env;
	while (previous_node->next != target_env)
		previous_node = previous_node->next;
	if (target_env->next)
		next_node = target_env->next;
	else
		next_node = NULL;
	free (target_env->var);
	free (target_env);
	previous_node->next = next_node;
	return (true);
}

int	unset_handler(t_shell *store)
{
	t_env	*current_env;
	char	*arg;

	current_env = store->env;
	if (!(store->cmd_head->command->next))
	{
		printf("unset: not enough arguments\n");
		return (BUILTIN_FAILURE);
	}
	arg = store->cmd_head->command->next->data;
	if (!is_valid_var_name(arg))
	{
		printf("unset: %s: invalid parameter name\n", arg);
		return (BUILTIN_FAILURE);	
	}
	if (got_equal(current_env, arg))
	{
		printf("unset: %s: invalid parameter name\n", arg);
		return (BUILTIN_FAILURE);
	}
	if (!perform_unset(arg, store))
		return (EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}