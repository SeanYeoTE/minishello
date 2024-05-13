
#include "minishell.h"

// split arg into char ** by spaces and detecting for double quotes,
// ignoring spaces when within quotes

static int	count_strings(char const *str, char c)
{
	int	i;
	int	count;
	int open;

	open = 0;
	count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		while (str[i] != '\0' && str[i] == c)
			i++;
		if (str[i] != '\0')
		{
			if (open % 2 != 0)
			{
				count--;
				i++;
			}
			count++;
		}
		while (str[i] != '\0' && !(str[i] == c))
		{
			if (str[i] == '"')
				open++;
			i++;
		}
	}
	return (count);
}

static char	**ft_freeup(char **strs)
{
	int	i;

	if (strs)
	{
		i = 0;
		while (strs[i] != NULL)
		{
			free(strs[i]);
			i++;
		}
		free(strs);
	}
	return (NULL);
}

static char	*ft_word(char const *str, char c)
{
	int		len_word;
	int		i;
	char	*word;
	int		open;

	open = 0;
	len_word = 0;
	i = 0;
	while (str[len_word])
	{
		if (str[len_word] == '"')
			open++;
		if (open % 2 == 0)
			break;
		else if (open % 2 == 1 && str[len_word])
			continue;
		len_word++;
	}
	word = (char *)malloc(sizeof(char) * (len_word + 1));
	if (!word)
		return (NULL);
	while (i < len_word)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static char	*ft_word(char const *str, char c)
	while (str[i])
	{
		if (str[i] == '"')
			i = ft_quotes(str[i]);
		else if (str[i] == ' ')
			i = ft_untilspace(str[i]);
		i++;
	}

int ft_quotes(char *str)
{
	int		i;
	int		occur;
	char	*word;
	int		j;
	i = 0;
	occur = 0;
	while (str[i])
	{
		if (str[i] == '"')
			occur++;
		if (occur == 2)
			return (i);
		i++;
	}
	word = (char *)malloc(sizeof(char) * (i + 1));
	if (!word)
		return (NULL);
	while (j < i)
	{
		word[j] = str[j];
		j++;
	}
	word[j] = '\0';
	return (word);
}

// cat "ls -l "




char	**ft_split_prompt(char const *str)
{
	char	c;
	char	ignore;
	int		i;
	int		open;

	open = 0;
	i = 0;
	c = ' ';
	ignore = '"';
	strings = (char **)malloc(sizeof(char *) * (count_strings(str, c)+ 1));
	if (!strings)
		return (NULL);
	while (*str)
	{
		while (*str != '\0' && (*str == c))
			str++;
		if (*str != '\0')
		{
			strings[i] = ft_word(str, c);
			if (strings[i] == NULL)
				return (ft_freeup(strings));
			i++;
		}
		while (*str && !(*str == c))
		{
			str++;
		}
	}
	strings[i] = 0;
	return (strings);
}