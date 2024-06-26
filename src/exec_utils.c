/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/26 14:06:36 by seayeo           ###   ########.fr       */
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
	int		exit_status;
	
	execveresult = 0;
	temp = argv_creator(start, end);
	while (start && start->type != 3)
		start = start->next;
	exepath = findprocesspath(store, temp);
	if (exepath == NULL)
	{
		perror("Path not found");
		free(exepath);
		exit_status = 127;
		return (exit_status);
	}
	dup2(store->output_fd, 1);
	dup2(store->input_fd, 0);
	execveresult = execve(exepath, temp, store->envp);
	if (execveresult == -1)
		exit_status = 127;
	if (exepath)
		free(exepath);
	free(temp);
	return (exit_status);
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
