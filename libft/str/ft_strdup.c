/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 17:00:34 by seayeo            #+#    #+#             */
/*   Updated: 2024/11/09 05:17:35 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char	*ft_strdup(const char *s)
{
	int		len;
	int		tmp;
	char	*str;

	len = ft_strlen(s);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	tmp = 0;
	while (tmp < len)
	{
		str[tmp] = s[tmp];
		tmp++;
	}
	str[tmp] = '\0';
	return (str);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;

	if (s == NULL || n <= 0)
		return (NULL);
	if (ft_strlen(s) < n)
		n = ft_strlen(s);
	dup = (char *)malloc((n + 1) * sizeof(char));
	ft_strlcpy(dup, s, n + 1);
	return (dup);
}
