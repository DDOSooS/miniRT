/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 20:24:10 by aghergho          #+#    #+#             */
/*   Updated: 2024/10/20 16:18:07 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minirt.h"
#include <string.h>

int ft_is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v');
}

double ft_atod(char *str)
{
    double result = 0.0;
    double fractional_part = 0.0;
    int i = 0;
    int sign = 1;
    double divisor ;

    while (ft_is_whitespace(str[i]))
        i++;
    if (str[i] == '-')
    {
        sign = -1;
        i++;
    } else if (str[i] == '+') 
        i++;
    while (str[i] && (str[i] >= '0' && str[i] <= '9'))
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    if (str[i] == '.')
    {
        i++; 
        divisor = 10.0;
        while (str[i] && (str[i] >= '0' && str[i] <= '9'))
        {
            fractional_part += (str[i] - '0') / divisor;
            divisor *= 10.0;
            i++;
        }
    }
    result += fractional_part;
    return result * sign;
}

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

int ft_is_degit(char *str)
{
    int i;

    i = 0;
    while (str[i] && (str[i] >= '0' && str[i] <= '9'))
         i++;
    if (str[i] == '.')
    {
        i++;
        while (str[i] && (str[i] >= '0' && str[i] <= '9'))
            i++;
        if (str[i])
            return 0;
    }
    return 1;
}

int ft_count_components(char **components)
{
    int i;

    i = 0;
    while (components[i])
        i++;
    return i;
}

int ft_check_range(char *component)
{
    double range;

    range = ft_atod(component);
    // printf("range = %f", range);
    if (range < 0 || range > 1)
        return 0;
    return 1;
}

int ft_check_colors(char *component)
{
    char **colors;
    int i;
    double color;
    
    colors = split(component,",");
    if (ft_count_components(colors) != 3)
        return (ft_free_line_components(colors), 0);
    if (!colors)
        return 0;
    i = -1;
    while (colors[++i])
    {
        color = ft_atod(colors[i]);
        if (color < 0 || color > 255)
            return (ft_free_line_components(colors), 0);         
    }
    return (1);
}

int ft_check_coordinates(char *component)
{
    char **coordinates;
    
    coordinates = split(component,",");
    if (ft_count_components(coordinates) != 3)
        return ( ft_free_line_components(coordinates),0);
    return ( ft_free_line_components(coordinates),1);
}

int ft_check_orientation(char *compoenent)
{
    char **oriontations;
    int i;
    double axes;
    
    i = -1;
    oriontations = split(compoenent, ",");
    if (ft_count_components(oriontations) != 3)
            return (ft_free_line_components(oriontations), 0);
    while (oriontations[++i])
    {
        axes = ft_atod(oriontations[i]);
        if (axes < -1 || axes > 1)
            return (ft_free_line_components(oriontations), 0);
    }
    return (1);
}

int ft_check_non_negative(char *component)
{
    double value;

    value = ft_atod(component);
    if (value < 0)
        return 0;
    return 1;
}

int ft_check_fov(char *component)
{
    double fov;

    fov = ft_atod(component);
    if (fov < 0 || fov > 180)
        return 0;
    return 1;
}

int ft_check_ambient_component(char **components, int *counter)
{
    if (ft_count_components(components) != 3)
        return 0;
    if (!ft_check_range(components[1]))
        return (0);
    if (!ft_check_colors(components[2]))
        return (0);
    (counter)[0] += 1;
    return (1);
}

int ft_check_camera_component(char **components, int *counter)
{
    if (ft_count_components(components) != 4)
        return (0);
      
    if (!ft_check_coordinates(components[1]))
        return (0);
    if (!ft_check_orientation(components[2]))
        return (0);
    if (!ft_check_fov(components[3]))
        return (0);
    (counter)[1]++;
    return (1);
}

int ft_check_light_component(char **components, int *counter)
{
    if (ft_count_components(components) != 4)
        return (0);
    if (!ft_check_coordinates(components[1]))
        return (0);
    if (!ft_check_range(components[2]))
        return (0);
    if (!ft_check_colors(components[3]))
        return (0);
    (counter)[2]++;
    // printf("light components are valid\n");
    return (1);
}

int ft_check_sphere_component(char **components, int *counter)
{
    if (ft_count_components(components) != 4)
        return 0;
    if (!ft_check_coordinates(components[1]))
        return 0;
    if (!ft_check_non_negative(components[2])) 
        return 0;
    if (!ft_check_colors(components[3]))
        return 0;
    // printf("sphere components are valid\n");
    return 1;
}

int ft_check_plane_component(char **components, int *counter)
{
    if (ft_count_components(components) != 4)
        return 0;
    if (!ft_check_coordinates(components[1]))
        return 0;
    if (!ft_check_orientation(components[2]))
        return 0;
    if (!ft_check_colors(components[3]))
        return 0;
    // printf("plane components are valid\n");
    return 1;
}

int ft_check_cylinder_component(char **components, int *counter)
{
    if (ft_count_components(components) != 5)
        return 0;
    if (!ft_check_coordinates(components[1]))
        return 0;
    if (!ft_check_orientation(components[2]))
        return 0;
    if (!ft_check_non_negative(components[3]) || !ft_check_non_negative(components[4]))
        return 0;
    if (!ft_check_colors(components[4]))
        return 0;
    // printf("cylinder components are valid\n");        
    return 1;
}

int ft_check_components(int identifier_id, char **components, int *counter)
{
    if (identifier_id == 1)
        return (ft_check_ambient_component(components, counter));
    if (identifier_id == 2)
        return (ft_check_camera_component(components , counter));
    if (identifier_id == 3)
        return (ft_check_light_component(components , counter));
    if (identifier_id == 4)
        return (ft_check_sphere_component(components , counter));
    if (identifier_id == 5)
        return (ft_check_plane_component(components , counter));
    if (identifier_id == 6)
        return (ft_check_cylinder_component(components , counter));
    return 1;
}
                                                                              
int is_identifier(char *identifier)
{
    if (!ft_strcmp(identifier, "A"))
        return 1;
    if (!ft_strcmp(identifier, "C"))
       return 2;
    if (!ft_strcmp(identifier, "L"))
        return 3;
    if (!ft_strcmp(identifier, "sp"))
        return 4;
    if (!ft_strcmp(identifier, "pl"))
        return 5;
    if (!ft_strcmp(identifier, "cy"))
        return 6;
    return 0;
}

int ft_check_map_components(t_map **map)
{
    map_line    *tmp;
    int         identifier_id;

    
    tmp = (*map)->lines;
    while (tmp)
    {
        identifier_id = is_identifier(tmp->line_component[0]);
        if (!identifier_id)
        {
            printf("identifier is not a valid identifier (%s)\n", tmp->line_component[0]);
            return (0);
        }
        if (!ft_check_components(identifier_id, tmp->line_component, ((*map)->scen_elements)))
        {
            printf("Error at map checking line component \n");
            return (0);
        }
        tmp = tmp->next;
    }


    if ((*map)->scen_elements[0] > 1 || (*map)->scen_elements[1] > 1
        || (*map)->scen_elements[2] > 1)
    {
        printf("duplicated of elements that must be declared just Once\n");
        return 0;
    }
    return (1);
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
}

map_line *ft_gen_scen_map(char *file_name)
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