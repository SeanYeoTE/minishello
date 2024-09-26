#include "minishell.h"

static bool	is_equal(t_env *current_env)
{
	int		i;

	i = 0;
	while (current_env->var[i])
	{
		if (current_env->var[i] == '=')
			return true;
		i++;
	}
	return false;
}

static void	perform_unset(t_env *target_env, t_shell *store)
{
	t_env	*previous_node;//node before target unset arg
	t_env	*next_node;//node after target unset arg

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

int	unset_handler(t_shell *store)
{
	t_env	*current_env;
	char	*arg;

	current_env = store->env;
	arg = store->cmd_head->command->next->data;
	if (!arg)
		printf("unset: not enough arguments\n");
	else if (!is_equal(current_env))
		printf("unset: %s: invalid parameter name\n", arg);
	else
	{
		current_env = get_target_loc(store->env, arg);
		perform_unset(current_env, store);
	}
	return (EXIT_SUCCESS);
}