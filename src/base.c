/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/25 17:53:26 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prompter(t_shell *store)
{
	char 	cwd[1024];
	char	*prompt;
	
	init_var(store);
	getcwd(cwd, sizeof(cwd));
	prompt = form_prompt(cwd);
	store->input = readline(prompt);
	if (!store->input)
		exit(EXIT_SUCCESS);
	if (store->input[0] == '\0')
		free_nonessential(store);
	add_history(store->input);
	if (!check_quotes(store->input))
		return (print_error("minishell: syntax error\n"));
	pre_execution(store, store->input);
	return (EXIT_SUCCESS);
}

int	pre_execution(t_shell *store, char *input)
{
	store->input = input_spacer(store->input);
	// printf("input: %s\n", input);
	if (ft_strchr(store->input, '$') != NULL)
		store->input = expansions(store->input);
	full_lexer(store->input, store, 0);
	parser(store);
	
	// print_stack(&store->head);
	
	return (EXIT_SUCCESS);
}

int		parser(t_shell* store)
{
	if (store->head)
	{
		if (pipe_counter(store->head) == 0)
			call_interpreter(store, store->head, store->tail);
		else if (pipe_counter(store->head) > 0)
			pre_interpreter(store, store->head);
		free_nonessential(store);
	}
	prompter(store);

	return (EXIT_SUCCESS);
}

void	call_interpreter(t_shell *store, t_node *start, t_node *end)
{
	int	pid1;
	
	if (start->data[0] == '/')
	{
		store->input_fd = 0;
		store->output_fd = 1;
	}
	else
	{
		if (check_builtin(start) == 0)
		{
			pid1 = fork();
			if (pid1 == 0)
			{
				if (check_builtin(start) == 0)
				{
					interpreter(store, start, end);
					exit(t_exit_status);
				}
			}
			else
				waitpid(pid1, &t_exit_status, WUNTRACED);
			if (WIFEXITED(t_exit_status))
				t_exit_status = WEXITSTATUS(t_exit_status);
		}
		else
			builtin_main(store, start, end);
	}
}	
void	interpreter(t_shell *store, t_node *loop, t_node *end)
{
	end = end->next;
	// print_stack_se(loop, end);
	while (loop != end)
	{
		// check if any pipes;

		// check if any redirection;
		if (redir_checker(loop) == 1)
			loop = redir_handler(store, loop, end);
		// check if any $ to expand;

		// normal executions;
		if (check_builtin(loop) == 0)
			loop = executor(store, loop, end);
		// exec builtins if any;
		// else
		// 	loop = builtin_main(store, loop, end);
	}
}


