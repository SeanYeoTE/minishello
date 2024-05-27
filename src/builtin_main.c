/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:40:05 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/27 15:50:43 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_main(t_shell *store, t_node	*current)
{
	if (current->data == "echo")
		echo_handler(current);
	if (current->data == "pwd")
		pwd_handler(current);
	else if (current->data == "|")
		pipe_handler(current);
}

// untested
t_node	*echo_handler(t_node *current)
{
	int	option;

	option = 0;
	current = current->next;
	if (ft_strncmp(current->data, "-n", 2) == 0)
		option = 1;
	if (option == 1)
		current = current->next;
	while (current)
	{
		printf("%c", current->data);
		printf(" ");
		current = current->next;
	}
	if (option == 0)
		printf("\n");
	return (current);
}

void	pwd_handler(t_shell *store)
{
	char	*cwd;
	size_t	cwd_buf;

	cwd = malloc (cwd_buf * sizeof(char));
	if (cwd == NULL)
		//handle error;
	if (getcwd(cwd, size) != NULL)
		ft_printf("%s\n", cwd);
	else
	{
		//handle error
		free (cwd);
	}
}

void	pipe_handler(t_shell *store)
{

}