#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"
#include <sys/wait.h>
#include <fcntl.h>

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
char		*form_prompt(char **envp, char *cwd);

// args_init.c
int 		init_node(char *value, t_node **head);
t_node		*get_last(t_node *last);
t_node		*get_node(t_node *ret, int num);

// main.c


// base.c
void		base_shell_init(t_shell *store, char *input);
int			check_builtin(t_shell *store, t_node *loop);
void		interpreter(t_shell *store);
int			redir_checker(t_shell *store, t_node *loop);


// exec_utils.c
char		*findprocesspath(t_shell *store, char **arr);
t_node		*executor(t_shell *store, t_node *current);
char		**argv_creator(t_node *current);

// printer.c
int 		print_stack(t_node **head, char c);
int			print_argv(char **argv);

// mem_utils.c
void		freechararray(char **v);
void		free_stack(t_node **stack);
void		free_nonessential(t_shell *store);

// builtin_main.c
t_node		*builtin_main(t_shell *store, t_node	*current);
t_node		*cd_handler(t_node *current);
t_node		*echo_handler(t_node *current);
t_node		*pwd_handler(t_node *current);

// parse_detection.c
int			detect_operator(char *str);
int			scanner_comment(char *str, int start, t_shell *store);
int			scanner_quote(char *str, int start, t_shell *store);
int			scanner_operator(char *str, int start, t_shell *store);
int			scanner_space(char *str, int start, t_shell *store);
int 		scanner_word(char *str, int start, t_shell *store);
int 		ft_sscan(char *str, t_shell *store, int index);

// redir.c

t_node		*redir_handler(t_shell *store, t_node *loop);
void		handle_output_redirection(t_shell *store, char *filename);
void		handle_append_redirection(t_shell *store, char *filename);
void		handle_input_redirection(t_shell *store, char *filename);
void		handle_heredoc_redirection(t_shell *store, char *filename);

#endif