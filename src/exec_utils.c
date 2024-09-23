/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/09/23 18:27:20 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*findprocesspath(t_shell *store, char **arr)
{
	int		i;
	char	*temp;
	char	*joined;

	i = 0;
	while ((store->paths)[i])
	{
		temp = ft_strjoin((store->paths)[i], "/");
		joined = ft_strjoin(temp, arr[0]);
		free(temp);
		if (access(joined, X_OK) == 0)
			return (joined);
		free(joined);
		i++;
	}
	return (NULL);
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
	if (!(ret = (char **)malloc(sizeof(char *) * (i + 1))))
		return (NULL);
	i = 0;
	while (start && start != end)
		ret[i++] = ft_strdup(start->data);
	ret[i] = NULL;
	return (ret);
}

static void	set_fd(t_cmd *node)
{
	if (node->input_fd != STDIN_FILENO)
	{
		if (dup2(node->input_fd, STDIN_FILENO) == -1)
			print_error("dup2 failed on input", NULL);
		close(node->input_fd);
	}
	if (node->output_fd != STDOUT_FILENO)
	{
		if (dup2(node->output_fd, STDOUT_FILENO) == -1)
			print_error("dup2 failed on output", NULL);
		close(node->output_fd);
	}
}

static void	cleanup(char *exepath, char **argv)
{
	int i;

	if (exepath)
		free(exepath);
	if (argv)
	{
		i = 0;
		while (argv[i])
			free(argv[i++]);
		free(argv);
	}
}

int	executor(t_shell *store, t_node *start, t_node *end)
{
	char	*exepath;
	char	**argv;
	
	if (!(argv = argv_creator(start, end)))
		print_error("Failed to create argv", NULL);
	if (!(exepath = findprocesspath(store, argv)))
	{
		cleanup(NULL, argv);
		print_error("Command not found", argv[0]);
	}
	set_fd(store->cmd_head);
	if (execve(exepath, argv, store->envp) == -1)
	{
		cleanup(exepath, argv);
		print_error("execve failed", NULL);
	}
	cleanup(exepath, argv);
	exit(EXIT_FAILURE);
}
