/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seayeo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 16:30:28 by seayeo            #+#    #+#             */
/*   Updated: 2024/04/27 16:10:50 by seayeo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	ft_chrindex(const char *s, int c)
{
	int		i;

	i = -1;
	while (s[++i])
		if (s[i] == (unsigned char)c)
			return (i);
	return (i);
}

char	*read_to_storage(int fd, char *store)
{
	char	*buf;
	int		ans;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	ans = 1;
	while (ans > 0)
	{
		ans = read(fd, buf, BUFFER_SIZE);
		if (ans < 0)
			return (free(store), free(buf), NULL);
		if (ans == 0)
			break ;
		buf[ans] = '\0';
		store = ft_strjoin_gnl(store, buf);
		if (ft_strchr_gnl(buf, '\n'))
			break ;
	}
	return (free(buf), store);
}

char	*extract_line(char *buf)
{
	char	*line;
	int		i;

	i = 0;
	if (!buf[i])
		return (NULL);
	i = ft_chrindex(buf, '\n');
	if (ft_strchr_gnl(buf, '\n'))
		i++;
	line = ft_substr_gnl(buf, 0, i);
	return (line);
}

char	*remains(char *buf)
{
	char	*leftover;
	int		cutoff;

	cutoff = ft_chrindex(buf, '\n');
	if (ft_strchr_gnl(buf, '\n'))
		cutoff++;
	if (buf[cutoff] == 0)
		return (free(buf), NULL);
	leftover = ft_substr_gnl(buf, cutoff, ft_strlen_gnl(buf) - cutoff);
	return (free(buf), leftover);
}

char	*get_next_line(int fd)
{
	static char	*buf[4096];
	char		*line;

	if (fd < 0 || fd > 4095 || BUFFER_SIZE <= 0)
		return (NULL);
	buf[fd] = read_to_storage(fd, buf[fd]);
	if (!buf[fd])
		return (NULL);
	line = extract_line(buf[fd]);
	buf[fd] = remains(buf[fd]);
	return (line);
}
