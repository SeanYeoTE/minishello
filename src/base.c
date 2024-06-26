/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 12:50:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/26 17:15:15 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prompter(t_shell *store)
{
	char 	cwd[1024];
	char	*prompt;
	
	signal(SIGINT, ctrl_c_handler);
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
	// print_stack(&store->head);
	parser(store);
	
	return (EXIT_SUCCESS);
}

int		parser(t_shell* store)
{
	if (store->head)
	{
		if (pipe_counter(store->head) == 0)
			single_function(store, store->head, store->tail);
		else if (pipe_counter(store->head) > 0)
			multiple_function(store);
	}
	free_nonessential(store);
	prompter(store);

	return (EXIT_SUCCESS);
}

int	multiple_function(t_shell *store)
{
	t_node	*temp;
	int		create;
	puts("multiple_function");
	temp = store->head;
	create = 0;
	while (temp)
	{
		if (ft_strcmp(temp->data, "|") == 0)
		{
			create_cmd(store, store->head, temp);
			temp = remove_node(store, store->head);
		}
		else
			temp = temp->next;
	}
	print_cmd_stack(&store->cmd_head);
}

int	create_cmd(t_shell *store, t_node *start, t_node *end)
{
	t_cmd	*new;
	t_node	*temp;
	
	if (start == NULL)
		return (1);
	else
	{
		init_cmd(store, start, end);
		return (0);
	}
}

void	detach_redir(t_cmd *new)
{
	t_node	*temp;
	puts("detach_redir");
	temp = new->command;
	while (temp)
	{
		if (redir_checker(temp) == 1)
		{
			new->redir = temp;
			temp->prev->next = new->redir->next->next;
			if (temp->next->next)
				temp->next->next->prev = temp->prev;
			
			new->redir->next->next = NULL;
			new->redir->prev = NULL;
		}
		temp = temp->next;
	}
}

int	single_function(t_shell *store, t_node *head, t_node *tail)
{
	int	pid1;
	
	create_cmd(store, head, tail);
	// puts("command\n");
	// print_stack(&store->cmd_head->command);
	// puts("redir\n");
	// print_stack(&store->cmd_head->redir);
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
		t_exit_status = builtin_main(store, store->cmd_head->command, NULL);
	}
	return (0);
}
		