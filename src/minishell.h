#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"
#include <sys/wait.h>

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
	char	**envp;
	
	char	*path;

	char	**paths;
	char	**argvs1;
	t_node	*head;

}	t_shell;

// prompt.c
char		*findpath(char *envp[]);
char		*finduser(char *envp[]);
char		*findhost(char *envp[]);

// args_init.c
int			init_node_stack(t_shell *store);
int 		init_node(char *value, t_node **head);
t_node	*get_last(t_node *last);

// main.c
int	single_execution(t_shell *store);
int base_shell_init(t_shell *store, char *input);

// exec_utils.c
char		*findprocesspath(t_shell *vars, int processnum);

// printer.c
int print_stack(t_node **head, char c);

// mem_utils.c
void	freechararray(char **v);
void	free_stack(t_node **stack);
void	free_nonessential(t_shell *store);

// builtin_main.c
int builtin_main(t_shell *store);

// split_quote.c
char	**ft_split_prompt(char const *str);


#endif