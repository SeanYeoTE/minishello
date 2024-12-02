/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 16:07:10 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 12:12:09 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Standard C library includes */
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <errno.h>

/* External library includes */
# include <readline/readline.h>
# include <readline/history.h>

/* System includes */
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>

/* Library includes */
# include "../libft/libft.h"

/* Project includes */
# include "types.h"
# include "core.h"
# include "builtins.h"
# include "execution.h"
# include "parser.h"
# include "lexer.h"
# include "redirections.h"
# include "utils.h"
# include "variables.h"

extern volatile sig_atomic_t	g_sig;

# define EXIT_SUCCESS 0
# define BUILTIN_FAILURE 1
# define BADCMD_FAILURE 126
# define NO_PERMISSION_FAILURE 127

#endif
