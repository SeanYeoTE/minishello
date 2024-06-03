/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/03 13:06:42 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	base_shell_init(t_shell *store, char *input)
{
	int		i;
	int		pid1;

	store->head = NULL;
	ft_sscan(input, store, 0);
	// print_stack(&store->head, 'a');
	pid1 = fork();
	if (pid1 == 0)
		interpreter(store);
	else
		free_nonessential(store);
	waitpid(pid1, NULL, 0);
}

void	interpreter(t_shell *store)
{
	t_node	*loop;
	
	loop = store->head;
	
	// check if any pipes;
	
	// check if any redirection;
	
	// check if any $ to expand;

	// normal executions;
	if (check_builtin(store, loop) == 0)
		executor(store, loop);
	// exec builtins if any;
	else
		builtin_main(store, loop);
		
	
}

int	check_builtin(t_shell *store, t_node *loop)
{
	if (ft_strcmp(loop->data, "echo") == 0)
		return (1);
	if (ft_strcmp(loop->data, "cd") == 0)
		return (1);
	if (ft_strcmp(loop->data, "pwd") == 0)
		return (1);	
	if (ft_strcmp(loop->data, "export") == 0)
		return (1);
	else
		return (0);
}

int	executor(t_shell *store, t_node *current)
{
	int		execveresult;
	char	*exepath;
	char	**temp;
	
	// puts("executing");
	execveresult = 0;
	temp = argv_creator(current);
	// print_argv(temp);
	exepath = findprocesspath(store, temp);
	if (exepath == NULL)
	{
		perror("Path not found");
		free(exepath);
		return (1);
	}
	// need to modify this as well
	execveresult = execve(exepath, temp, store->envp);
	if (execveresult == -1)
		perror("execve error");
	if (exepath)
		free(exepath);
	free(temp);
	return (0);
}

// pls check this function, not sure if it works as intended
// intention is to create an argv array for execve
char	**argv_creator(t_node *current)
{
	int		i;
	t_node	*temp;
	char	**ret;
	
	i = 0;
	temp = current;
	while (temp && temp->type != 3)
	{
		temp = temp->next;
		i++;
	}
	ret = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (current && current->type != 3)
	{
		ret[i] = ft_strdup(current->data);
		// printf("ret[%d]: %s\n", i, ret[i]);
		current = current->next;
		i++;
	}
	ret[i] = '\0';
	return (ret);
}