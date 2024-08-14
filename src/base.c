/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/08/12 15:16:54 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prompter(t_shell *store, t_env *env_head, t_var *var_head)
{
	char 	cwd[1024];
	char	*prompt;
	
	signal(SIGINT, ctrl_c_handler);
	init_var(store, env_head, var_head);
	getcwd(cwd, sizeof(cwd));
	prompt = form_prompt(cwd);
	store->input = readline(prompt);
	if (store->input == NULL)
		exit(EXIT_SUCCESS);
	if (store->input[0] == '\0')
	{
		free_nonessential(store);
		prompter(store, env_head, var_head);
	}
	add_history(store->input);
	if (!check_quotes(store->input))
		return (print_error("minishell: syntax error\n"));
	pre_execution(store, NULL);
	return (EXIT_SUCCESS);
}

int	pre_execution(t_shell *store, char *input)
{
	store->input = input_spacer(store->input);
	printf("input: %s\n", store->input);
	if (ft_strchr(store->input, '$') != NULL)
		store->input = expansions(store->input);
	full_lexer(store->input, store, 0);
	print_stack(&store->head);
	parser(store);
	
	return (EXIT_SUCCESS);
}

int		parser(t_shell* store)
{
	t_env	*env_head;
	t_var	*var_head;

	env_head = store->env;
	var_head = store->var;
	if (store->head)
	{
		if (pipe_counter(store->head) == 0)
			single_function(store, store->head, store->tail);
		else if (pipe_counter(store->head) > 0)
			multiple_function(store, pipe_counter(store->head));
	}
	free_nonessential(store);
	prompter(store, env_head, var_head);

	return (EXIT_SUCCESS);
}

int	multiple_function(t_shell *store, int count)
{
	t_node	*front;
	t_node 	*back;
	t_node	*temp;
	bool	create;
	
	// puts("multiple_function");
	front = store->head;
	back = store->head;
	create = true;
	store->pid = ft_calloc(sizeof(int), count + 2);
	while (back->next)
	{
		if (ft_strcmp(back->data, "|") == 0)
		{
			temp = back->next;
			create_cmd(store, front, back, create);
			create = false;
			front = temp;
			back = temp;
		}
		else
			back = back->next;
	}
	create_cmd(store, front, back->prev, create);
	// print_cmd_stack(&store->cmd_head);
	multi_executor(store, count_cmds(store) - 1);
	revert_nodes(store);
	return (0);
}



int	single_function(t_shell *store, t_node *head, t_node *tail)
{
	int	pid1;
	
	create_cmd(store, head, tail, true);
	if (check_builtin(store->cmd_head->command) == 0)
	{
		pid1 = fork();
		if (pid1 == 0)
		{
			redir_handler(store, store->cmd_head->redir, NULL);
			t_exit_status = executor(store, store->cmd_head->command, NULL);
			exit(t_exit_status);
		}
		else
			waitpid(pid1, &t_exit_status, WUNTRACED);
		if (WIFEXITED(t_exit_status))
			t_exit_status = WEXITSTATUS(t_exit_status);
	}
	else
	{
		// need to change builtiin main; currently still functioning on the old method of
		// linked lists, would not function as expected when redirections are required
		t_exit_status = builtin_main(store, store->cmd_head->command, NULL);
		exit(t_exit_status);
	}
	return (t_exit_status);
}
		