/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/27 15:20:17 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	base_shell_init(t_shell *store, char *input)
{
	int		i;
	int		pid1;

	store->head = NULL;
	ft_sscan(input, store, 0);
	print_stack(&store->head, 'a');
	pid1 = fork();
	if (pid1 == 0)
	{
		interpreter(store);
	}
	else
		free_nonessential(store);
	waitpid(pid1, NULL, 0);
	return (0);
}

void	interpreter(t_shell *store)
{
	int		i;
	t_node	*loop;
	i = 0;
	store->input_fd = STDIN_FILENO;
	store->output_fd = STDOUT_FILENO;

	loop = store->head;
	while (loop)
	{
		if (loop->type == 5)
		{
			builtin_main(store, loop);
		}
		loop = loop->next;
	}

	
	
}

int	executor(t_shell *store)
{
	int		execveresult;
	char	*exepath;
	
	execveresult = 0;
	// need to modify this function
	exepath = findprocesspath(store, 1);
	
	if (exepath == NULL)
	{
		perror("Path not found");
		free(exepath);
		return (1);
	}
	// need to modify this as well 
	execveresult = execve(exepath, store->head, store->envp);
	if (execveresult == -1)
		perror("execve error");
	if (exepath)
		free(exepath);
	return (0);
}
// need to break this down to call exec functions, not call from within itself
// example: this should execute the pipex function
// currently it doesnt 
// int	single_execution(t_shell *store)
// {
// 	int 	execveresult;
// 	char	*exepath;

// 	execveresult = 0;
// 	exepath = findprocesspath(store, 1);
// 	if (exepath == NULL)
// 	{
// 		perror("Path not found");
// 		free(exepath);
// 		return (1);
// 	}
// 	execveresult = execve(exepath, ft_split(store->head->data), store->envp);
// 	if (execveresult == -1)
// 		perror("execve error");
// 	if (exepath)
// 		free(exepath);
// 	return (0);
// }


// if (strcmp(store->head->data, "cd") == 0)
// 		{
// 			if (cd(store->head->next->data) < 0)
// 				perror(input);
// 		}
	
int cd(char *path) {
	return chdir(path);
}