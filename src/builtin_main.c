/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 12:40:05 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/28 17:10:51 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*builtin_main(t_shell *store, t_node *current)
{
	t_node	*ret;
	if (current->data == "cd")
		ret = cd_handler(current);
	else if (current->data == "echo")
		ret = echo_handler(current);
	// else if (current->data == "pwd")
	// 	ret = pwd_handler(current);
	// else if (current->data == "|")
	// 	ret = pipe_handler(current);
}

// untested
t_node	*cd_handler(t_node *current)
{
	if (chdir(current->next->data) < 0)
		perror(current->data);
	return (current);
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
		printf("%s", current->data);
		printf(" ");
		current = current->next;  
	}
	if (option == 0)
		printf("\n");
	return (current);
}

// void	pwd_handler(t_shell *store)
// {
// 	char	*cwd;
// 	size_t	cwd_buf;

// 	cwd = malloc (cwd_buf * sizeof(char));
// 	if (cwd == NULL)
// 		//handle error;
// 	if (getcwd(cwd, size) != NULL)
// 		ft_printf("%s\n", cwd);
// 	else
// 	{
// 		//handle error
// 		free (cwd);
// 	}
// }

// void	pipe_handler(t_shell *store)
// {

// }