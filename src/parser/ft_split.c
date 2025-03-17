/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:49:09 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 10:20:43 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	check_delimiter(char c, char *delimiter)
{
	int	i;

	i = 0;
	while (delimiter[i])
	{
		if (c == delimiter[i])
			return (1);
		i++;
	}
	return (0);
}

static int	ft_count_words(char const *s, char *delimiter)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && check_delimiter(s[i], delimiter))
			i++;
		if (s[i])
			count++;
		while (s[i] && !check_delimiter(s[i], delimiter))
			i++;
	}
	return (count);
}

static char	*ft_genword(char const *s, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!word)
		return (NULL);
	while (start < end)
		word[i++] = s[start++];
	word[i] = '\0';
	return (word);
}

static void	ft_gen_words(char **words, char const *s, char *delimiter)
{
	int	i;
	int	start;
	int	k;

	i = 0;
	k = 0;
	while (s[i])
	{
		if (!check_delimiter(s[i], delimiter))
		{
			start = i;
			while (s[i] && !check_delimiter(s[i], delimiter))
				i++;
			words[k] = ft_genword(s, start, i);
			if (!words[k])
				return (ft_free(&words, k));
			k++;
		}
		else
			i++;
	}
	words[k] = NULL;
}

char	**ftt_split(char const *s, char *delimiter)
{
	char	**words;

	if (!s)
		return (NULL);
	words = (char **)malloc(sizeof(char *) * (ft_count_words(s, delimiter)
				+ 1));
	if (!words)
		return (NULL);
	ft_gen_words(words, s, delimiter);
	return (words);
}
