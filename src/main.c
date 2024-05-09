/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/09 13:59:33 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// need to break this down to call exec functions, not call from within itself
// example: this should execute the pipex function
// currently it doesnt 
int	single_execution(t_shell *store)
{
	int 	execveresult;
	char	*exepath;

	execveresult = 0;
	exepath = findprocesspath(store, 1);
	if (exepath == NULL)
	{
		perror("Path not found");
		free(exepath);
		return (1);
	}
	execveresult = execve(exepath, store->argvs1, store->envp);
	if (execveresult == -1)
		perror("execve error");
	if (exepath)
		free(exepath);
	return (0);
}

// ft_split of args should initialise and then next step is
// to store the seperate args in struct nodes 
// so that the ast can be built
// perhaps first i can init as a stack, then make a parser to 
// convert stack into ast
int base_shell_init(t_shell *store, char *input)
{
	int		i;
	int		pid1;

	store->argvs1 = ft_split(input, ' ');
	store->head = NULL;
	init_node_stack(store);
	// print_stack(&store->head, 'a');
	store->paths = ft_split(store->path + 5, ':');
	if (store->head->next == NULL)
	{
		pid1 = fork();
		if (pid1 == 0)
			single_execution(store);
		else
		{
			free_nonessential(store);
		}
		waitpid(pid1, NULL, 0);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	store;
	char	*path;
	char	*hostname;
	char	*username;
	char	*input;

	char 	cwd[1024];
	if (argc != 1)
	{
		perror("run without args");
		return (0);
	}
	store.envp = envp;
	getcwd(cwd, sizeof(cwd));
	store.path = findpath(envp);
	username = finduser(envp);
	hostname = findhost(envp);
	while (1)
	{	
		if (isatty(STDOUT_FILENO))
		{
			printf("%s@%s:~%s$ ",username, hostname, cwd);
			input = readline(" ");
			base_shell_init(&store, input);
			// printf("%s\n", input);
		}
	}
	return (0);
}
