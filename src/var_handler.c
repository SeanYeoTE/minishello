#include "minishell.h"

t_var	*create_var_node(char *var, char *data)
{
	t_var	*new_var;

	new_var = (t_var *)malloc(sizeof(t_var));
	if (!new_var)
		perror("Failed to allocate memory");
	new_var->name = ft_strdup(var);
	new_var->data = ft_strdup(data);
	new_var->hidden = NULL;
	new_var->next = NULL;
	return (new_var);
}

static int	count_src(char *src, int flag)
{
	int		i;
	char	*temp;

	i = 0;
	temp = NULL;
	if (flag == 1)
	{
		while (*src != '=')
		{
			src++;
			i++;
		}
	}
	else
	{
		temp = ft_strchr(src, '=');
		while (*temp != '\0')
		{
			temp++;
			i++;
		}
	}
	return (i);
}

t_var	*split_var(char *src)
{
	int	i;
	int	j;
	t_var	*new_var;
	char	*name;
	char	*value;


	i = 0;
	j = 0;
	name = malloc(sizeof(char) * (count_src(src, 1) + 1));
	value = malloc(sizeof(char) * (count_src(src, 2) + 1));
	while (src[i] != '=')
	{
		name[i] = src[j];
		i++;
		j++;
	}
	name[i] = '\0';
	i = 0;
	while (src[j] != '\0')
	{
		j++;
		value[i] = src[j];
		i++;
	}
	value[i] = '\0';
	// while ((src[i] != '=') && src[i])
	// {
	// 	name[i] = src[i];
	// 	i++;
	// }
	// name[i] = '\0';
	// while (src[i])
	// {
	// 	i++;
	// 	value[j] = src[i];
	// 	j++;
	// }
	new_var = create_var_node(name, value);
	new_var->hidden = ft_strdup(src);
	free (name);
	free (value);
	return (new_var);
}

int	var_handler(char *src, t_shell *store)
{
	t_var	*new_var;
	t_var	*current;

	new_var = NULL;
	if (!same_env(src, store) || !same_var(src, store))
		new_var = split_var(src);
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
	return (EXIT_SUCCESS);
}