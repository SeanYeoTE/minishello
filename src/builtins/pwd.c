/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchua <mchua@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 21:32:36 by mchua             #+#    #+#             */
/*   Updated: 2024/11/08 21:32:36 by mchua            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	pwd_handler(void)
{
	char	*cwd;
	size_t	cwd_buf;

	cwd_buf = 1000;
	cwd = malloc (cwd_buf * sizeof(char));
	if (cwd == NULL)
	{
		perror("Invalid Memory Allocation\n");
		return (1);
	}
	if (getcwd(cwd, cwd_buf) != NULL)
		printf("%s\n", cwd);
	else
	{
		perror("PWD Error\n");
		free(cwd);
		return (BUILTIN_FAILURE);
	}
	free(cwd);
	return (EXIT_SUCCESS);
}
