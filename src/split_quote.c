
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
		while (str[i] != '\0' && (str[i] == c || str[i] == '"'))
			i++;
		if (str[i] != '\0')
		{
			if (str[i] == '"')
			{
				open++;
				if (open % 2 == 0)
					count++;
			}
			else
			{
				if (open % 2 == 0)
					count++;
			}
			i++;
		}
		while (str[i] != '\0' && !(str[i] == c || str[i] == '"'))
			i++;
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

char	*ft_quotes(char const *str)
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
			break;
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

char	*ft_untilspace(char const *str)
{
	int		i;
	char	*word;
	int		j;

	i = 0;
	j = 0;
	while (str[i] && !(str[i] == ' '))
		i++;
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

static char	*ft_word(char const *str, char c)
{
	int		i;
	char	*word;
	
	i = 0;
	while (str[i])
	{
		if (str[i] == '"')
			return (ft_quotes(str));
		else if (str[i] == ' ')
			return (ft_untilspace(str));
		i++;
	}
}
char	**ft_split_prompt(char const *str)
{
	char	c;
	char	ignore;
	int		i;
	int		open;
	char	**strings;

	open = 0;
	i = 0;
	c = ' ';
	ignore = '"';
	printf("%d\n", count_strings(str, c));
	strings = (char **)malloc(sizeof(char *) * (count_strings(str, c)+ 1));
	if (!strings)
		return (NULL);
	while (*str)
	{
		while (*str != '\0' && (*str == c || *str == ignore))
			str++;
		if (*str != '\0')
		{
			strings[i] = ft_word(str, c);
			if (strings[i] == NULL)
				return (ft_freeup(strings));
			i++;
		}
		while (*str && !(*str == c || *str == ignore))
			str++;
	}
	strings[i] = 0;
	return (strings);
}