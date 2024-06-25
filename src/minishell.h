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
#include <signal.h>

// global variable
extern	int		t_exit_status;

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

// struct points to command and redirection, each is a linked list
// struct points to next command as itself is a linked list
typedef struct s_cmd
{

	char	*data;
	
	t_node 	*command;
	t_node	*redir;
	struct s_cmd	*next;
	struct s_cmd	*prev;

}	t_cmd;

typedef struct s_env
{
	char	*var;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	char	**envp;
	int		input_fd;
	int		output_fd;

	char	*input;

	char	*path;
	char	**paths;

	t_cmd	*cmd_head;
	t_cmd	*cmd_tail;

	t_node	*head;
	t_node	*tail;
	t_env	*env;
}	t_shell;

///////////////////////////////////////////
// the ui portion //
// main.c
void		free_env(t_env **env);
void		init_var(t_shell *store);
int			main(int argc, char **argv, char **envp);
int			looper(t_shell *store);

// input_utils.c
char		*input_spacer(char *input);
int			check_quotes(char *line);
int 		redir_checker(t_node *loop);

// prompt.c
char		*form_prompt(char *cwd);
///////////////////////////////////////////
// lexer //
// parse_detection.c
int			detect_operator(char *str);
int			check_builtin(t_node *loop);
int 		full_lexer(char *str, t_shell *store, int index);

// scanner.c
int			scanner_comment(char *str, int start, t_shell *store);
int			scanner_quote(char *str, int start, t_shell *store);
int			scanner_operator(char *str, int start, t_shell *store);
int			scanner_space(char *str, int start);
int 		scanner_word(char *str, int start, t_shell *store);

// args_init.c
int			init_node(char *value, t_node **head);
t_node		*get_last(t_node *last);
t_node		*get_node(t_node *ret, int num);
///////////////////////////////////////////
// parser //
// base.c
int			prompter(t_shell *store);
int			pre_execution(t_shell *store, char *input);
int			parser(t_shell *store);
void		interpreter(t_shell *store, t_node *start, t_node *end);
int			multiple_function(t_shell *store);
void		create_cmd(t_shell *store, t_node *start, t_node *end);







// exec_utils.c
char		*findprocesspath(t_shell *store, char **arr);
t_node		*executor(t_shell *store, t_node *start, t_node *end);
char		**argv_creator(t_node *start, t_node *end);

// printer.c
int 		print_stack(t_node **head);
int 		print_stack_se(t_node *start, t_node *end);
int			print_argv(char **argv);
int			print_error(char *str);

// mem_utils.c
void		freechararray(char **v);
void		free_stack(t_node **stack);
void		free_nonessential(t_shell *store);

// builtin_main.c
t_node		*builtin_main(t_shell *store, t_node *current, t_node *end);
t_node		*cd_handler(t_node *current);
t_node		*echo_handler(t_node *current, t_node *end);
t_node		*pwd_handler(t_node *current);
t_env		*create_env_node(char *env_var);
void		env_init(t_shell *store, char **envp);
void		env_handler(t_shell *store);

// redir.c

t_node		*redir_handler(t_shell *store, t_node *loop, t_node *end);
void		handle_output_redirection(t_shell *store, char *filename, t_node *start, t_node *end);
void		handle_append_redirection(t_shell *store, char *filename, t_node *start, t_node *end);
void		handle_input_redirection(t_shell *store, char *filename, t_node *start, t_node *end);
void		handle_heredoc_redirection(t_shell *store, char *filename);

// pipe.c

int 		pipe_counter(t_node *loop);
void    	pre_interpreter(t_shell *store, t_node *temp);
void		single_function(t_shell *store, t_node *start, t_node *end);
t_node		*pipe_slicer(t_node *tail);
t_node		*get_start(t_node *start, int i);
t_node		*get_end(t_node *start, int i);

//sig_handler.c
// void		reg_ctrl_c(void);
void		ctrl_c_handler(int signum);

// expansions.c
char		*expansions(char *input);

#endif