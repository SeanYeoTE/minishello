#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../../libft/libft.h"
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>

#define	EXIT_SUCCESS 0
#define BUILTIN_FAILURE 1
#define BADCMD_FAILURE 126
#define	NO_PERMISSION_FAILURE 127

// global variable
extern	sig_atomic_t	t_exit_status;

typedef struct s_node
{	
	int 	type;
	int		opprio;

	char 	*data;

	struct s_cmd	*parent;
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
	bool	input_changed;

	pid_t	pid;
	int		input_fd;
	int		output_fd;
	int		heredoc_fd;
	char	*heredoc_delimiter;
	int		pipe_out;
	
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
	int		input_reset;
	int		output_reset;

	int		fd_in;

	bool	quotes;
	bool	expanded;

	char	*input;

	char	*path;
	char	**paths;

	t_cmd	*cmd_head;
	t_cmd	*cmd_tail;
	int		pipes;

	t_node	*head;
	t_node	*tail;
	t_env	*env;
	t_var	*var;
}	t_shell;

// Function prototypes

// main.c

void		init_var(t_shell *store, t_env *env_head, t_var *var_head);
int			main(int argc, char **argv, char **envp);

// input_utils.c
char		*input_spacer(char *input);

// prompt.c
char		*form_prompt(char *cwd);

// checks.c
int 		redir_checker(t_node *cmd);
int			check_builtin(t_node *loop);
int			is_operator(char c);
int			is_double_operator(const char *input, int i);

// checks2.c
int			check_quotes(char *line);
int			check_error(char *input);

// expansions.c
char		*expansions(char *input);

// remove_quote.c
void		remove_quote(t_node *token);

// parse_detection.c
bool 		is_space(char c);
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

// base.c
int			prompter(t_shell *store, t_env *env_head, t_var *var_head);
int			pre_execution(t_shell *store);
int			parser(t_shell *store);
void		interpreter(t_shell *store, t_node *start, t_node *end);
int			multiple_function(t_shell *store);
int			single_function(t_shell *store, t_node *start, t_node *end);

// t_cmd_utils.c
t_cmd		*get_last_cmd(t_cmd *cmd);
t_cmd		*init_cmd(t_shell *store, t_node *start, t_node *end, bool create);
int			create_cmd(t_shell *store, t_node *start, t_node *end, bool create);
void		detach_redir(t_cmd *new);
int			count_cmds(t_shell *store);

// exec_utils.c
void		check_open_fds(int max_fd);
int			executor(t_shell *store, t_cmd *cmd);

// printer.c
int 		print_stack(t_node **head);
int 		print_stack_se(t_node *start, t_node *end);
int			print_argv(char **argv);
int			print_cmd_stack(t_cmd **head);
int			print_error(char *str, char *arg);
void		print_erroronly(char *str, char *arg);

// mem_utils.c
void		free_nonessential(t_shell *store);
void		free_all(t_shell *store);

// builtin_main.c
int			builtin_main(t_shell *store, t_node *current);

// cd.c
int			cd_handler(t_node *current);

// echo.c
int			echo_handler(t_node *current);

// pwd.c
int			pwd_handler(void);

// env.c
t_env		*create_env_node(char *var);
t_env		*env_init(char **envp);
int			env_handler(t_shell *store);

// export.c
t_env		*get_env_loc(t_env *env_list, char *arg);
t_var		*get_var_loc(char *arg, t_var *var_list, t_env *current_env);
int			export_handler(t_shell *store);

// export_utils.c
int			args_key_counter(char *src);
bool		is_in_env(t_env *env_list, char *arg);
t_env		*get_last_env(t_env *current_env);

// var_handler.c
t_var		*create_var_node(char *var, char *data);
t_var		*split_var(char *src);
int			var_handler(char *src, t_shell *store);

// var_utils.c
int			print_var(t_var *var);
int			name_counter(char *src);
bool		same_env(char *src, t_shell *store);
bool		same_var(char *src, t_shell *store);

// unset.c
int			unset_handler(t_shell *store);

// exit.c
int			exit_handler(t_shell *store);

// redir.c
void		reset_fds(t_shell *store);
int			redir_handler(t_cmd *cmd, t_node *loop, t_node *end);
int			handle_output_redirection(t_cmd *cmd, char *filename);
int			handle_append_redirection(t_cmd *cmd, char *filename);
int			handle_input_redirection(t_cmd *cmd, char *filename);
int			handle_heredoc_redirection(t_cmd *cmd, char *delimiter);

// pipe.c
int			wait_for_command(pid_t pid);
int 		pipe_counter(t_node *loop);
int			multi_executor(t_shell *store);

// sig_handler.c
void		ctrl_c_handler(int signum);

// heredoc.c
int			handle_heredoc(t_cmd *cmd);
int			handle_heredoc_pipe(t_cmd *cmd);

#endif