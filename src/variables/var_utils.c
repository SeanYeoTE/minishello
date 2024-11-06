#include "../core/minishell.h" //to add into header file

int print_var(t_var *var)
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

int	name_counter(char *src)
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

bool	same_env(char *src, t_shell *store)
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

bool	same_var(char *src, t_shell *store)
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