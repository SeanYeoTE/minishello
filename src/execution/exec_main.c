/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/10 09:27:21 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../core/minishell.h"

static int	handle_execution_errors(char *exepath, char **argv,
	char *temp_filename)
{
	int	dir;

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

static char	*resolve_path(t_shell *store, char **argv)
{
	if (ft_strncmp(argv[0], "./", 2) == 0 || ft_strchr(argv[0], '/'))
		return (ft_strdup(argv[0]));
	return (findprocesspath(store, argv));
}

static int	run_execve(t_shell *store, char *exepath, char **argv)
{
	if (execve(exepath, argv, store->envp) == -1)
	{
		free_all(store);
		print_erroronly(strerror(errno), argv[0]);
		return (126);
	}
	return (EXIT_SUCCESS);
}

int	executor(t_shell *store, t_cmd *cmd)
{
	char	*exepath;
	char	**argv;
	char	*temp_filename;
	int		error_code;

	temp_filename = NULL;
	argv = argv_creator(cmd->command, NULL);
	if (!argv)
		return (EXIT_FAILURE);
	exepath = resolve_path(store, argv);
	error_code = handle_execution_errors(exepath, argv, temp_filename);
	if (error_code != 0)
	{
		free_all(store);
		return (error_code);
	}
	set_fd(cmd, temp_filename);
	check_open_fds(100);
	error_code = run_execve(store, exepath, argv);
	if (error_code != EXIT_SUCCESS)
		cleanup(exepath, argv, temp_filename);
	return (error_code);
}
