/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 20:24:10 by aghergho          #+#    #+#             */
/*   Updated: 2024/10/18 16:55:52 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minirt.h"
#include   <string.h>

void ft_free_line_components(char **components)
{
    int i;

    i = 0;
    while (components[i])
    {
        free(components[i]);
        i++;
    }
    free(components);
}

void    ft_free_map(map_line **map_lines)
{
    map_line *tmp;

    while (*map_lines)
    {
        tmp = (*map_lines)->next;
        ft_free_line_components((*map_lines)->line_component);
        free(*map_lines);
        (*map_lines) = tmp;
    }
    free(map_lines);
    map_lines = NULL;
}

int ft_is_whitespace(const char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v');
}

int ft_check_file_name(char *filename)
{
    char *extention;

    extention = ft_strrchr(filename, '.');
    if (extention && ft_strcmp(extention, ".rt") == 0)
        return 1;
    return 0;
}

int check_delimiter(char c, char *delimiter)
{
    int    i;

    i = 0;
    while (delimiter[i])
    {
        if (c == delimiter[i])
            return (1);
        i++;
    }
    return (0);
}

static void	ft_free(char ***words, int size)
{
	int	i;

	i = -1;
	while (++i < size)
		free((*words)[i]);
	free(*words);
	*words = NULL;
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

char	**split(char const *s, char *delimiter)
{
	char	**words;

	if (!s)
		return (NULL);
	words = (char **)malloc(sizeof(char *) * (ft_count_words(s, delimiter) + 1));
	if (!words)
		return (NULL);
	ft_gen_words(words, s, delimiter);
	return (words);
}
/*=======================Testing part==================
int	main(void)
{
	char	*s;
	char	**words;

	s = "hello worldsgfdsgdsffffg|||||||||";
	words = ft_split("nonempty", 0);
	for (int i = 0; words[i]; i++)
	{
			printf("%s\n", words[i]);
	        printf("Length: %zu\n", strlen(words[i]));
	}
	return (0);
}
===================================================================*/


                                                                              {
    
}

int ft_check_map_contents(t_map **map)
{
    map_line    *tmp;
    int         identifier_id;

    
    tmp = *map;
    while (tmp)
    {
        identifier_id = is_identifier(tmp->line_component[0]);
        if (identifier_id == -1)
            return (0);
        if (!ft_check_components(identifier_id))
            return (0);
        tmp = tmp->next;
    }
    
}



//================================================================
void    var_dump_lines(map_line *map)
{
    if (!map)
    {
        printf("map is null\n");
        return ;
    }

    printf("============== map  =================\n");
    while (map)
    {
        // printf("line : %s\n", map->lin);
        for (int i = 0; map->line_component[i]; i++)
            printf("component %d : %s\n", i, map->line_component[i]);
        printf("======================================\n");
        printf("\n");
        map = map->next;
    }
    printf("======================================\n");
}


map_line    *ft_get_last_line(map_line **map)
{
    map_line    *tmp;

    tmp = *map;
    if (!tmp)
        return (NULL);
    while (tmp->next)
        tmp = tmp->next;
    return (tmp);
}

int    ft_add_line(map_line **map, char *line)
{
    map_line    *new;
    map_line    *last;


    new = (map_line *)malloc(sizeof(map_line));
    if (!new)
        return 0;
    new->line_component = split(line, " \t\n\r\v");
    
    new->next = NULL;
    last = ft_get_last_line(map);
    if (last)
        last->next = new;
    else
        *map = new;
    return (1);
    // new->l
    // printf(">> line : %s<<\n", line);
    // for (size_t i = 0; new->line_component[i] ; i++)
    // {
    //     printf(" -> (%s) <-", new->line_component[i]);
    // }
    // printf("\n");
}


map_line *ft_gen_sen_map(char *file_name)
{
    map_line *map;
    char *line;
    int fd;

    if (!ft_check_file_name(file_name))
        return (printf("error : invalid file extension\n"),NULL);
    fd = open(file_name, O_RDONLY);
    if (fd < 0)
        return (printf("error : failed to open file\n"),NULL);
    map = NULL;
    line = get_next_line(fd);
    while (line)
    {
        if (line && strncmp(line, "\n", 1))
            if (!ft_add_line(&map, line))
                return (free(line), close(fd),NULL);
        free(line);
        line = get_next_line(fd);
    }
    close(fd);

    return (map);
}