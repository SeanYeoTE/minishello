#include "minishell.h"

int	args_key_counter(char *src)
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

bool	is_in_env(t_env *env_list, char *arg, t_shell *store)
{
	t_env	*current_env;

	current_env = get_env_loc(env_list, arg);
	if (!current_env)
		return (false);
	return (true);
}

t_env	*get_last_env(t_env *current_env)
{
	t_env	*last_env;

	last_env = current_env;
	while (last_env->next)
		last_env = last_env->next;
	return (last_env);
}