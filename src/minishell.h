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
	int		input_fd;
	int		output_fd;

	char	*path;
	char	**paths;
	t_node	*head;

}	t_shell;

// prompt.c
char		*findpath(char *envp[]);
char		*finduser(char *envp[]);
char		*findhost(char *envp[]);

// args_init.c
int 		init_node(char *value, t_node **head);
t_node		*get_last(t_node *last);
t_node		*get_node(t_node *ret, int num);

// main.c


// base.c
void		base_shell_init(t_shell *store, char *input);
int			check_builtin(t_shell *store, t_node *loop);
char		**argv_creator(t_node *current);
void		interpreter(t_shell *store);
int			executor(t_shell *store, t_node *current);

// exec_utils.c
char		*findprocesspath(t_shell *store, char **arr);

// printer.c
int 		print_stack(t_node **head, char c);

// mem_utils.c
void		freechararray(char **v);
void		free_stack(t_node **stack);
void		free_nonessential(t_shell *store);

// builtin_main.c
t_node		*builtin_main(t_shell *store, t_node	*current);
t_node		*cd_handler(t_node *current);
t_node		*echo_handler(t_node *current);

// parse_detection.c
int			detect_operator(char *str);
int			scanner_comment(char *str, int start, t_shell *store);
int			scanner_quote(char *str, int start, t_shell *store);
int			scanner_operator(char *str, int start, t_shell *store);
int			scanner_space(char *str, int start, t_shell *store);
int 		scanner_word(char *str, int start, t_shell *store);
int 		ft_sscan(char *str, t_shell *store, int index);

#endif