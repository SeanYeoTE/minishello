/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/08/05 13:02:37 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// takes in the struct and processnum int, returning
// full path for execution
char	*findprocesspath(t_shell *store, char **arr)
{
	int		i;
	char	*temp;
	char	*joined;

	i = 0;
	joined = NULL;
	while ((store->paths)[i])
	{
		temp = ft_strjoin((store->paths)[i], "/");
        joined = ft_strjoin(temp, arr[0]);
		if (access(joined, X_OK) == 0)
			break ;
		free(joined);
		joined = NULL;
		i++;
	}
	free(temp);
	return (joined);
}

// fd issues, output not redirecting properly
int	executor(t_shell *store, t_node *start, t_node *end)
{
	int		execveresult;
	char	*exepath;
	char	**temp;
	
	execveresult = 0;
	temp = argv_creator(start, end);
	while (start && start->type != 3)
		start = start->next;
	exepath = findprocesspath(store, temp);
	if (exepath == NULL)
	{
		perror("Path not found");
		free(exepath);
		t_exit_status = 127;
		return (t_exit_status);
	}
	dup2(store->output_fd, 1);
	dup2(store->input_fd, 0);
	execveresult = execve(exepath, temp, store->envp);
	if (execveresult == -1)
		t_exit_status = 127;
	if (exepath)
		free(exepath);
	free(temp);
	return (t_exit_status);
}

int multi_executor(t_shell *store, int num_pipes)
{
	t_cmd   *temp;
	int     pipefd[num_pipes][2];
	int     count;
	int     pids[num_pipes];
	count = 0;
	t_exit_status = 0;
	temp = store->cmd_head;
	while (temp)
	{
		pipe(pipefd[count]);
		if (count > 0)
			store->input_fd = pipefd[count - 1][0];
		store->output_fd = pipefd[count][1];
		if (check_builtin(temp->command) == 0)
		{
			pids[count] = fork();
			if (pids[count] == 0)
			{
				redir_handler(store, temp->redir, NULL);
				t_exit_status = executor(store, temp->command, NULL);
				exit(t_exit_status);
			}
			else
			{
				temp = temp->next;
			}
			count++;
		}
		else
		{
			t_exit_status = builtin_main(store, store->cmd_head->command, NULL);
		}
		count++;
	}
	int i = 0;
	while (i < num_pipes)
	{
		close(pipefd[i][0]);
		close(pipefd[i][1]);
		i++;
	}
	i = 0;
	while (i < num_pipes)
	{
		waitpid(pids[i], &t_exit_status, WUNTRACED);
		i++;
	}
	return (t_exit_status);
}

// intention is to create an argv array for execve
char	**argv_creator(t_node *start, t_node *end)
{
	int		i;
	t_node	*temp;
	char	**ret;
	
	i = 0;
	temp = start;
	while (temp && temp != end)
	{
		temp = temp->next;
		i++;
	}
	ret = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (start && start != end)
	{
		ret[i] = ft_strdup(start->data);
		start = start->next;
		i++;
	}
	ret[i] = '\0';
	return (ret);
}
