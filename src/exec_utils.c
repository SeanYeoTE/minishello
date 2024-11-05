/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/05 22:09:08 by seayeo           ###   ########.fr       */
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
	ret = (char **)ft_calloc(i + 2, sizeof(char *));
	if (!ret)
		return (NULL);
	i = 0;
	while (start && start != end)
	{
		ret[i] = ft_strdup(start->data);
		if (!ret[i])
		{
			while (--i >= 0)
				free(ret[i]);
			free(ret);
			return (NULL);
		}
		start = start->next;
		i++;
	}
	ret[i] = NULL;
	return (ret);
}

static void	set_fd(t_cmd *node, char *temp_filename)
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
	else if (temp_filename)
	{
		node->input_fd = open(temp_filename, O_RDONLY);
		if (node->input_fd == -1)
			print_error("Failed to open temporary file", strerror(errno));
		else if (dup2(node->input_fd, STDIN_FILENO) == -1)
			print_error("dup2 failed on temporary input", strerror(errno));
		// close(node->input_fd);
	}
	if (node->output_fd != STDOUT_FILENO)
	{
		if (dup2(node->output_fd, STDOUT_FILENO) == -1)
			print_error("dup2 failed on output", strerror(errno));
		close(node->output_fd);
	}
}

static void cleanup(char *exepath, char **argv, char *temp_filename)
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
	if (temp_filename)
	{
		// unlink(temp_filename);
		// free(temp_filename);
	}
}

int is_directory(const char *path)
{
	struct stat	path_stat;
	if (stat(path, &path_stat) != 0)
		return (-1);
	return(S_ISDIR(path_stat.st_mode));
}

static char	*get_temp_filename(int index)
{
	char	*filename;
	char	*index_str;
	index_str = ft_itoa(index);
	filename = ft_strjoin("/tmp/tmp_", index_str);
	free(index_str);
	return (filename);
}

static char *handle_cat_without_args(t_cmd *cmd, int index)
{
	char *temp_filename;
	int fd;
	temp_filename = get_temp_filename(index);
	fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		print_error("Failed to create temporary file", strerror(errno));
		free(temp_filename);
		return NULL;
	}
	cmd->input_fd = STDIN_FILENO;
	return temp_filename;
}

static int handle_execution_errors(char *exepath, char **argv, char *temp_filename)
{
	int dir;

	if (!exepath)
	{
		print_erroronly("command not found", argv[0]);
		cleanup(NULL, argv, temp_filename);
		return (127);
	}

	dir = is_directory(exepath);
	if (dir == -1)
	{
		print_erroronly(strerror(errno), argv[0]);
		cleanup(exepath, argv, temp_filename);
		return (127);
	}
	if (dir == 1)
	{
		print_erroronly("Is a directory", argv[0]);
		cleanup(exepath, argv, temp_filename);
		return (126);
	}

	return (0);
}

void check_open_fds(int max_fd)
{
    for (int fd = 3; fd <= max_fd; fd++)
    {
        if (fcntl(fd, F_GETFD) != -1 || errno != EBADF)
        {
   			close(fd);
            // printf("File descriptor %d is open\n", fd);
        }
    }
}

int		executor(t_shell *store, t_cmd *cmd, int index)
{
	char	*exepath;
	char	**argv;
	char    *temp_filename = NULL;
	int     error_code;

	argv = argv_creator(cmd->command, NULL);
	if (!argv)
		return (EXIT_FAILURE);
	// if (cmd != NULL && cmd->prev != NULL)
	// {
	// }
	// else if (cmd->input_fd == STDIN_FILENO) //cmd->input_changed == false) //|| cmd->output_fd == STDOUT_FILENO)
	// {
		// if (cmd->output_fd != 3 && ft_strcmp(argv[0], "cat") == 0 && argv[1] == NULL)
		// 	temp_filename = handle_cat_without_args(cmd, index);
	// }
	if (ft_strncmp(argv[0], "./", 2) == 0 || ft_strchr(argv[0], '/'))
		exepath = ft_strdup(argv[0]);
	else
		exepath = findprocesspath(store, argv);

	error_code = handle_execution_errors(exepath, argv, temp_filename);
	if (error_code != 0)
		return error_code;

	set_fd(cmd, temp_filename);
	check_open_fds(100);
	if (execve(exepath, argv, store->envp) == -1)
	{
		print_erroronly(strerror(errno), argv[0]);
		cleanup(exepath, argv, temp_filename);
		return (126);
	}
	return (EXIT_SUCCESS);
}
