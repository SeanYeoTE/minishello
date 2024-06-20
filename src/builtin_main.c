/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:40:05 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/20 13:53:13 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*builtin_main(t_shell *store, t_node *current, t_node *end)
{
	t_node	*ret;

	(void)store;
	
	if (ft_strcmp(current->data, "cd") == 0)
		ret = cd_handler(current);
	else if (!ft_strcmp(current->data, "echo"))
		ret = echo_handler(current, end);
	else if (!ft_strcmp(current->data, "pwd"))
		ret = pwd_handler(current);
	return (ret);
}

// untested
t_node	*cd_handler(t_node *current)
{
	char	*home;

	if (current->next == NULL || ft_strcmp(current->next->data, "~") == 0)
	{
		home = getenv("HOME");
		if (home == NULL || chdir(home) != 0)
			perror("no home");
	}
	else if (chdir(current->next->data) != 0)
		perror(current->data);
	return (current);
}

// untested
t_node	*echo_handler(t_node *current, t_node *end)
{
	int	option;	

	option = 0;
	current = current->next;
	if (ft_strncmp(current->data, "-n", 2) == 0)
		option = 1;
	if (option == 1)
		current = current->next;
	while (current != end)
	{
		printf("%s", current->data);
		printf(" ");
		current = current->next;  
	}
	if (option == 0)
		printf("\n");
	return (current);
}

t_node	*pwd_handler(t_node *current)
{
	char	*cwd;
	size_t	cwd_buf;

	cwd_buf = 1000;
	cwd = malloc (cwd_buf * sizeof(char));
	if (cwd == NULL)
		perror("Invalid Memory Allocation\n");
	if (getcwd(cwd, cwd_buf) != NULL)
		printf("%s\n", cwd);
	else
	{
		perror("PWD Error\n");
		free (cwd);
	}
	return (current->next);
}
// void	pipe_handler(t_shell *store)
// {

// }