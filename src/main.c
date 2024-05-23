/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/23 16:41:58 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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


// ft_split of args should initialise and then next step is
// to store the seperate args in struct nodes 
// so that the ast can be built
// perhaps first i can init as a stack, then make a parser to 
// convert stack into ast
int base_shell_init(t_shell *store, char *input)
{
	int		i;
	int		pid1;

	store->head = NULL;
	ft_sscan(input, store, 0);
	print_stack(&store->head, 'a');
	// pid1 = fork();
	// if (pid1 == 0)
	// {
		
	// }
	// else
	// 	free_nonessential(store);
	// waitpid(pid1, NULL, 0);

	
	return (0);
}

// scrolling up for history works, but scrolling down after messes up the prompt
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
	username = finduser(envp);
	hostname = findhost(envp);
	while (1)
	{	
		getcwd(cwd, sizeof(cwd));
		store.path = findpath(envp);
		
		if (isatty(STDOUT_FILENO))
		{
			printf("%s@%s:~%s$ ",username, hostname, cwd);
			input = readline(" ");
			add_history(input);
			base_shell_init(&store, input);
			free(input);
		}
	}
	return (0);
}
