/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/05 13:34:40 by seayeo           ###   ########.fr       */
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

t_node	*executor(t_shell *store, t_node *current)
{
	int		execveresult;
	char	*exepath;
	char	**temp;
	
	// puts("executing");
	execveresult = 0;
	temp = argv_creator(current);
	while (current && current->type != 3)
		current = current->next;
	// print_argv(temp);
	exepath = findprocesspath(store, temp);
	if (exepath == NULL)
	{
		perror("Path not found");
		free(exepath);
		return (current);
	}
	dup2(store->output_fd, 1);
	dup2(store->input_fd, 0);
	// need to modify this as well
	execveresult = execve(exepath, temp, store->envp);
	if (execveresult == -1)
		perror("execve error");
	if (exepath)
		free(exepath);
	free(temp);
	return (current);
}

// intention is to create an argv array for execve
char	**argv_creator(t_node *current)
{
	int		i;
	t_node	*temp;
	char	**ret;
	
	i = 0;
	temp = current;
	while (temp && temp->type != 3)
	{
		temp = temp->next;
		i++;
	}
	ret = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (current && current->type != 3)
	{
		ret[i] = ft_strdup(current->data);
		current = current->next;
		i++;
	}
	ret[i] = '\0';
	return (ret);
}
