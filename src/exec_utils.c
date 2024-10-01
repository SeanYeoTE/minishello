/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/01 16:33:15 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*findprocesspath(t_shell *store, char **arr)
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

static char	**argv_creator(t_node *start, t_node *end)
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
	ret = (char **)malloc(sizeof(char *) * (i + 2)); // Allocate one extra for NULL terminator
	if (!ret)
		return (NULL);
	i = 0;
	while (start && start != end)
	{
		ret[i] = ft_strdup(start->data);
		if (!ret[i])
		{
			// Clean up if strdup fails
			while (--i >= 0)
				free(ret[i]);
			free(ret);
			return (NULL);
		}
		start = start->next;
		i++;
	}
	ret[i] = NULL; // Null-terminate the array
	return (ret);
}

static void	set_fd(t_cmd *node)
{
	if (node->heredoc_fd != -1)
	{
		if (dup2(node->heredoc_fd, STDIN_FILENO) == -1)
			print_error("dup2 failed on heredoc input", strerror(errno));
		close(node->heredoc_fd);
		node->heredoc_fd = -1;
	}
	else if (node->input_fd != STDIN_FILENO)
	{
		if (dup2(node->input_fd, STDIN_FILENO) == -1)
			print_error("dup2 failed on input", strerror(errno));
		close(node->input_fd);
	}
	if (node->output_fd != STDOUT_FILENO)
	{
		if (dup2(node->output_fd, STDOUT_FILENO) == -1)
			print_error("dup2 failed on output", strerror(errno));
		close(node->output_fd);
	}
}

static void cleanup(char *exepath, char **argv)
{
	if (exepath)
		free(exepath);
	if (argv)
	{
		int i = 0;
		while (argv[i])
			free(argv[i++]);
		free(argv);
	}
}

int	executor(t_shell *store, t_node *start, t_node *end)
{
	char	*exepath;
	char	**argv;
	
	argv = argv_creator(start, end);
	if (!argv)
		return (EXIT_FAILURE);
	if (!(exepath = findprocesspath(store, argv)))
	{
		print_error("Command not found", *argv);
		cleanup(NULL, argv);
		return (EXIT_FAILURE);
	}
	set_fd(store->cmd_head);	
	if (execve(exepath, argv, store->envp) == -1)
	{
		cleanup(exepath, argv);
		print_error("execve failed", strerror(errno));
		return (EXIT_FAILURE);
	}
	cleanup(exepath, argv);
	return (EXIT_SUCCESS);
}
