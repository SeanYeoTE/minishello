/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 18:11:01 by seayeo            #+#    #+#             */
/*   Updated: 2024/12/02 12:14:09 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h" 

volatile sig_atomic_t	g_sig;

static int			get_lvl(const char *str)
{
	int	i;
	int	sign;
	int	num;

	i = 0;
	sign = 1;
	num = 0;
	// ft_skip_spacenl(str, &i);
	// if (invalid_lvl(str))
	// 	return (0);
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
		num = num * 10 + (str[i++] - '0');
	return (num * sign);
}

static void	increment_shl_level(t_env *env)
{
	int		shell_level;
	char	**shell_value;
	char	*shlvl;

	shlvl = NULL;
	shell_value = NULL;
	while (env && env->next)
	{
		if (ft_strncmp("SHLVL", env->var, 5) == 0)
		{
			shell_value = ft_split(env->var, '=');
			shell_level = get_lvl(shell_value[1]) + 1;
			free(shell_value[1]);
			shell_value[1] = NULL;
			shlvl = ft_itoa(shell_level);
			env->var = ft_strjoin(shell_value[0], "=");
			env->var = ft_strjoin(env->var, shlvl);
			return ;
		}
		env = env->next;
	}
	if (shell_value)
		freechararray(shell_value);
	free(shlvl);
}
/**
 * @brief Entry point of the minishell program
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @param envp Array of environment variables
 * @return Exit status of the shell program
 * @details Initializes the shell environment and starts the main command loop.
 *          Validates that no command line arguments are provided, as the shell
 *          should be run without arguments. Sets up the environment variables
 *          and launches the main shell prompt.
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell			store;
	t_env			*env_head;
	t_shell_state	state;

	if (argc != 1 || argv[1])
	{
		perror("run without args");
		return (0);
	}
	env_head = env_init(envp);
	increment_shl_level(env_head);
	init_shell_state(&state, env_head);
	return (minishell_loop(&store, &state));
}

void	init_shell_state(t_shell_state *state, t_env *env_head)
{
	state->env = env_head;
	state->var = NULL;
	state->exit_status = 0;
}
