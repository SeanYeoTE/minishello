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
	int		opprio;

	char 	*data;
	struct s_node	*next;
	struct s_node	*prev;

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
	char	**argvs1;
	char	**argvs2;
	t_node	*head;

}	t_shell;


char		*findpath(char *envp[]);
char		*finduser(char *envp[]);
char		*findhost(char *envp[]);

int			init_node_stack(t_shell *store);
int 		init_node(char *value, t_node **head);
t_node	*get_last(t_node *last);


char		*findprocesspath(t_shell *vars, int processnum);

int print_stack(t_node **head, char c);

#endif