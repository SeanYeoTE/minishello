/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/10/18 18:35:50 by seayeo           ###   ########.fr       */
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
		if (!temp)
			return (NULL);
		joined = ft_strjoin(temp, arr[0]);
		free(temp);
		if (!joined)
			return (NULL);
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
	// ret = (char **)malloc(sizeof(char *) * (i + 2)); // Allocate one extra for NULL terminator
	ret = (char **)ft_calloc(i + 2, sizeof(char *));
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
		// printf("node output_fd: %d\n", node->output_fd);
		// write(1, "hello\n", 6);
		// write(6, "15e64\n", 6);
		// printf("STDOUT_FILENO: %d\n", STDOUT_FILENO);
		if (dup2(node->output_fd, STDOUT_FILENO) == -1)
			print_error("dup2 failed on output22", strerror(errno));
		close(node->output_fd);
	}
	// printf("node output_fd: %d\n", node->output_fd);
	// printf("final STDOUT_FILENO: %d\n", STDOUT_FILENO);
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

int is_directory(const char *path)
{
	struct stat	path_stat;
	if (stat(path, &path_stat) != 0)
		return (-1); // Indicates an error in getting the file status
	return(S_ISDIR(path_stat.st_mode));
}

int	executor(t_shell *store, t_cmd *cmd)
{
	char	*exepath;
	char	**argv;
	int		dir;
	
	argv = argv_creator(cmd->command, NULL);
	if (!argv)
		return (EXIT_FAILURE);
	// Check if the command is a local script or contains a path
	if (ft_strncmp(argv[0], "./", 2) == 0 || ft_strchr(argv[0], '/'))
		exepath = ft_strdup(argv[0]);
	else
		exepath = findprocesspath(store, argv);
	if (!exepath)
	{
		print_erroronly("command not found", argv[0]);
		cleanup(NULL, argv);
		// return (EXIT_FAILURE);
		return (127);
	}
	set_fd(cmd);
	dir = is_directory(exepath);
	if (dir == -1)
	{
		print_erroronly(strerror(errno), argv[0]);
		cleanup(exepath, argv);
		return (127);
	}
	if (dir == 1)
	{
		print_erroronly("Is a directory", argv[0]);
		cleanup(exepath, argv);
		return (126);
	}
	if (execve(exepath, argv, store->envp) == -1)
	{
		print_erroronly(strerror(errno), argv[0]);
		cleanup(exepath, argv);
		return (126);
	}
	cleanup(exepath, argv);
	return (EXIT_SUCCESS);
}
