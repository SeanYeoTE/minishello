#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"

typedef struct s_var
{	
	int 	type;
	char 	data;
	int		opprio;

} t_var;

typedef struct s_shell
{
	t_var 	*head;
	t_var 	*leftchild;
	t_var 	*rightchild;

	char	cwd[1024];
}	t_shell;


#endif