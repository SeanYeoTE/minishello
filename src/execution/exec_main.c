/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/18 13:09:16 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 
/**
 * @brief Handles various execution error cases
 *
 * @param exepath Path to the executable
 * @param argv Array of command arguments
 * @param temp_filename Temporary file name for redirection
 * @return int Error code (0 for success, 126 for permission denied,
 * 			127 for not found)
 * @note Checks for command existence, directory status, and permissions
 */
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

/**
 * @brief Resolves the full path of a command
 *
 * @param store Shell data structure containing environment
 * @param argv Array of command arguments
 * @return char* Full path to executable, NULL if not found
 * @note Handles both relative paths (./command) and PATH lookup
 */
static char	*resolve_path(t_shell *store, char **argv)
{
	if (argv[0] == NULL)
		return (NULL);
	if (ft_strncmp(argv[0], "./", 2) == 0 || ft_strchr(argv[0], '/'))
		return (ft_strdup(argv[0]));
	return (findprocesspath(store, argv));
}

/**
 * @brief Executes a command using execve
 *
 * @param store Shell data structure containing environment
 * @param exepath Path to the executable
 * @param argv Array of command arguments
 * @return int Exit status (0 for success, 126 for execution error)
 * @note Frees shell resources before exec failure
 */
static int	run_execve(t_shell *store, char *exepath, char **argv)
{
	if (execve(exepath, argv, store->envp) == -1)
	{
		free_all(store);
		if (errno == EACCES)
			print_erroronly("permission denied", argv[0]);
		else if (errno == EBADF)
			return (126);
		else
			print_erroronly(strerror(errno), argv[0]);
		return (126);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Main executor function for running commands
 *
 * @param store Shell data structure containing environment
 * @param cmd Command structure containing command details
 * @return int Exit status of command execution
 * @note Handles command setup, error checking, and execution
 */
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
