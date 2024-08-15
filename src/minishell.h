#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#include <errno.h>

// global variable
extern	int		t_exit_status;

typedef struct s_node
{	
	int 	type;
	int		opprio;

	char 	*data;

	struct s_node	*next;
	struct s_node	*prev;
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

typedef struct s_var
{
	char	*name;
	char	*data;
	char	*hidden;
	struct s_var	*next;
}	t_var;

typedef struct s_shell
{
	char	**envp;
	int		input_fd;
	int		output_fd;

	bool	quotes;

	char	*input;

	char	*path;
	char	**paths;

	t_cmd	*cmd_head;
	t_cmd	*cmd_tail;
	int		*pid;
	int		pipes;

	t_node	*head;
	t_node	*tail;
	t_env	*env;
	t_var	*var;
}	t_shell;

///////////////////////////////////////////
// the ui portion //
// main.c
void		free_env(t_env **env);
void		init_var(t_shell *store, t_env *env_head, t_var *var_head);
int			main(int argc, char **argv, char **envp);
int			looper(t_shell *store);

// input_utils.c
char		*input_spacer(char *input);

// prompt.c
char		*form_prompt(char *cwd);
///////////////////////////////////////////
// lexer //
// checks.c
int			check_quotes(char *line);
int 		redir_checker(t_node *cmd);
int			check_builtin(t_node *loop);
int			is_operator(char c);
int			is_double_operator(const char *input, int i);

// expansions.c
char		*expansions(char *input);

// remove_quote.c
void		remove_quote(t_node *token);

// parse_detection.c
int			detect_operator(char *str);
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
void		revert_nodes(t_shell *store);
///////////////////////////////////////////
// parser //
// base.c
int			prompter(t_shell *store, t_env *env_head, t_var *var_head);
int			pre_execution(t_shell *store, char *input);
int			parser(t_shell *store);
void		interpreter(t_shell *store, t_node *start, t_node *end);
int			multiple_function(t_shell *store, int count);
int			single_function(t_shell *store, t_node *start, t_node *end);

// t_cmd_utils.c
t_cmd		*get_last_cmd(t_cmd *cmd);
t_cmd		*init_cmd(t_shell *store, t_node *start, t_node *end, bool create);
int			create_cmd(t_shell *store, t_node *start, t_node *end, bool create);
void		detach_redir(t_cmd *new);
int			count_cmds(t_shell *store);

// exec_utils.c
char		*findprocesspath(t_shell *store, char **arr);
int			executor(t_shell *store, t_node *start, t_node *end);
char		**argv_creator(t_node *start, t_node *end);


// printer.c
int 		print_stack(t_node **head);
int 		print_stack_se(t_node *start, t_node *end);
int			print_argv(char **argv);
int			print_cmd_stack(t_cmd **head);
int			print_error(char *str);

// mem_utils.c
void		freechararray(char **v);
void		free_stack(t_node **stack);
void		free_nonessential(t_shell *store);

// builtin_main.c
int			builtin_main(t_shell *store, t_node *current, t_node *end);
int			cd_handler(t_node *current);
int			echo_handler(t_node *current, t_node *end);
int			pwd_handler(t_node *current);
t_env		*create_env_node(char *var);
t_env		*env_init(t_shell *store, char **envp);
void		env_handler(t_shell *store);
t_var		*create_var_node(char *var, char *data);
t_var		*split_var(char *src, t_var *var);
t_var		*var_init(char *src, char *name, char *value, t_var *var);
int			var_handler(char *src, t_var **var);
int			export_handler(t_env *env, t_var *var);

// redir.c

t_node		*redir_handler(t_shell *store, t_node *loop, t_node *end);
void		handle_output_redirection(t_shell *store, char *filename, t_node *start, t_node *end);
void		handle_append_redirection(t_shell *store, char *filename, t_node *start, t_node *end);
void		handle_input_redirection(t_shell *store, char *filename, t_node *start, t_node *end);
void		handle_heredoc_redirection(t_shell *store, char *filename);

// pipe.c

int 		pipe_counter(t_node *loop);
int			multi_executor(t_shell *store, int	num_pipes);

//sig_handler.c
// void		reg_ctrl_c(void);
void		ctrl_c_handler(int signum);



#endif