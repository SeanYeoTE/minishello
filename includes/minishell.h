/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:07:10 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/14 23:49:44 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdbool.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include <sys/stat.h>

# define EXIT_SUCCESS 0
# define BUILTIN_FAILURE 1
# define BADCMD_FAILURE 126
# define NO_PERMISSION_FAILURE 127

extern volatile sig_atomic_t g_sig;

/* Type definitions */
typedef struct s_quote_state
{
	bool	in_single_quotes;
	bool	in_double_quotes;
}	t_quote_state;

typedef struct s_node
{
	int				type;
	int				opprio;
	char			*data;
	struct s_cmd	*parent;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;

typedef struct s_cmd
{
	char			*data;
	t_node			*command;
	t_node			*redir;
	bool			input_changed;
	pid_t			pid;
	int				input_fd;
	int				output_fd;
	int				heredoc_fd;
	int				heredoc_write_fd;
	char			*heredoc_delimiter;
	char			*heredoc_filename;
	int				pipe_out;
	struct s_cmd	*next;
	struct s_cmd	*prev;
}	t_cmd;

typedef struct s_env
{
	char			*var;
	struct s_env	*next;
}	t_env;

typedef struct s_var
{
	char			*name;
	char			*data;
	char			*hidden;
	struct s_var	*next;
}	t_var;

typedef struct s_shell_state
{
	t_env	*env;
	t_var	*var;
	int		exit_status;
}	t_shell_state;

typedef struct s_shell
{
	char	**envp;
	int		exit_status;
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

# include "core.h"
# include "builtins.h"
# include "execution.h"
# include "parser.h"
# include "lexer.h"
# include "redirections.h"
# include "utils.h"
# include "variables.h"

#endif
