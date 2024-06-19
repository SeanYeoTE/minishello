/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 15:34:18 by seayeo            #+#    #+#             */
/*   Updated: 2024/06/19 16:13:30 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char	*expansions(char *input)
{
	int		i;
	int		j;
	char	*front;
	char	*back;
	char	*var;
	char	*temp;
	int		changed;

	i = 0;
	while (input[i])
	{
		changed = 1;
		if (input[i] == '$')
		{
			j = i + 1;
			while ((ft_isalnum(input[j]) || input[j] == '_') && input[j] != '\0' )
				j++;
			var = ft_substr(input, i + 1, j - i - 1);
			temp = getenv(var);
			if (temp == NULL)
			{
				changed = 0;
				temp = "";
			}
			front = ft_substr(input, 0, i);
			back = ft_substr(input, j, ft_strlen(input) - j);
			input = ft_strjoin(front, ft_strjoin(temp, back));
			free(front);
			free(back);
			free(var);
		}
		i++;
	}
	if (changed == 1)
		printf("%s: is a directory\n", input);
	return (input);
}