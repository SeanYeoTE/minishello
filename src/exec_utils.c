/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 13:41:40 by seayeo            #+#    #+#             */
/*   Updated: 2024/05/23 16:50:48 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// takes in the struct and processnum int, returning
// full path for execution
// char	*findprocesspath(t_shell *vars, int processnum)
// {
// 	int		i;
// 	char	*temp;
// 	char	*joined;

// 	i = 0;
// 	joined = NULL;
// 	while ((vars->paths)[i])
// 	{
// 		temp = ft_strjoin((vars->paths)[i], "/");
// 		if (processnum == 1)
// 			joined = ft_strjoin(temp, vars->head->data);
// 		if (access(joined, X_OK) == 0)
// 			break ;
// 		free(joined);
// 		joined = NULL;
// 		i++;
// 	}
// 	free(temp);
// 	return (joined);
// }