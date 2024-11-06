#include "../core/minishell.h"

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

t_env	*env_init(char **envp)
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

int	env_handler(t_shell *store)
{	
	t_env	*current;

	current = store->env;
	while (current)
	{
		ft_putstr_fd(current->var, store->cmd_head->output_fd);
		ft_putstr_fd("\n", store->cmd_head->output_fd);
		current = current->next;
	}
	return (EXIT_SUCCESS);
}