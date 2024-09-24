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
	if (previous_node->next->next != NULL)
		next_node = previous_node->next->next;
	else
		next_node = NULL;
	free (target_env->var);
	free (target_env);
	previous_node->next = next_node;
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
		current_env = get_env_loc(store->env, arg);
		perform_unset(current_env, store);
	}
	return (EXIT_SUCCESS);
}