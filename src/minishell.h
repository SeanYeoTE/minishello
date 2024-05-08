#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"

typedef struct s_node
{	
	int 	type;
	char 	data;
	int		opprio;

	struct s_node 	*leftchild;
	struct s_node 	*rightchild;
	struct s_node	*parent;

} t_node;

typedef struct s_shell
{
	// char	cwd[1024];
	// char	*hostname;
	// char	*username;
	
	char	**envp;
	
	char	*path;
	char	**paths;
	char	**argv;
	t_node	*head;

}	t_shell;


char	*findpath(char *envp[]);
char	*finduser(char *envp[]);
char	*findhost(char *envp[]);

#endif