

#include "minishell.h"


int	init_node_stack(t_shell *store)
{
	int	i;

	i = 0;
	while (store->argvs1[i])
	{
		init_node(store->argvs1[i], &store->head);
		i++;
	}
	return (0);
}

int init_node(char *value, t_node **head)
{
	t_node	*new_node;
	t_node	*prev_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return (-1);
	new_node->next = NULL;
	new_node->data = value;
	if (*head == NULL)
	{
		new_node->prev = NULL;
		*head = new_node;
	}
	else
	{
		prev_node = get_last(*head);
		prev_node->next = new_node;
		new_node->prev = prev_node;
	}
	return (0);
}

t_node	*get_last(t_node *last)
{
	while (last->next != NULL)
		last = last->next;
	return (last);
}
