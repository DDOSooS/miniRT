/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 20:24:10 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/04 16:00:19 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/minirt.h"
#include <string.h>

int ft_is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v');
}

double get_fractional(char *str, int i)
{
    double fractional_part;
    double divisor ;
    
    fractional_part = 0.0;
    divisor = 10.0;
    while (str[++i] && (str[i] >= '0' && str[i] <= '9'))
    {
        fractional_part += (str[i] - '0') / divisor;
        divisor *= 10.0;
    }
    return fractional_part;
}

double ft_atod(char *str)
{
    double result = 0.0;
    int i = 0;
    int sign = 1;

    while (ft_is_whitespace(str[i]))
        i++;
    if (str[i] == '-')
    {
        sign = -1;
        i++;
    }
    else if (str[i] == '+') 
        i++;
    i--;
    while (str[++i] && (str[i] >= '0' && str[i] <= '9'))
        result = result * 10 + (str[i] - '0');
    if (str[i] == '.')
        result += get_fractional(str, i);
    return result * sign;
}

int is_empty_line(char *line)
{   
    int i = 0;
    
    if (!line || line[0] == '\n')
        return 1;

    while (line[i])
    {
        if (!ft_is_whitespace(line[i]))
            return 0;
        i++;
    }
    return 1;
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
    if (str[i] == '+' || str[i] == '-')
        i++;
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
    else if (str[i])
        return 0;
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
    if (!colors)
        return 0;
    if (ft_count_components(colors) != 3)
        return (ft_free_line_components(colors), 0);
    i = -1;
    while (colors[++i])
    {
        if (!ft_is_degit(colors[i]))
            return (ft_free_line_components(colors), 0);
        color = ft_atod(colors[i]);
        if (color < 0 || color > 255)
            return (ft_free_line_components(colors), 0);         
    }
    ft_free_line_components(colors);
    return (1);
}

int ft_check_elements(char *component)
{
    char **coordinates;
    int i;

    i = -1;
    coordinates = split(component,",");
    if (ft_count_components(coordinates) != 3)
        return ( ft_free_line_components(coordinates),0);
    while (coordinates[++i])
    {
        if (!ft_is_degit(coordinates[i]))
            return (ft_free_line_components(coordinates), 0);
    }
    return ( ft_free_line_components(coordinates),1);
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
    if (!ft_check_elements(components[1]))
        return (0);
    if (!ft_check_elements(components[2]))
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
    if (!ft_check_elements(components[1]))
        return (0);
    if (!ft_check_range(components[2]))
        return (0);
    if (!ft_check_colors(components[3]))
        return (0);
    (counter)[2]++;
    // printf("light components are valid\n");
    return (1);
}

int ft_check_sphere_component(char **components)
{
    if (ft_count_components(components) != 4)
        return 0;
    if (!ft_check_elements(components[1]))
        return 0;
    if (!ft_check_non_negative(components[2])) 
        return 0;
    if (!ft_check_colors(components[3]))
        return 0;
    // printf("sphere components are valid\n");
    return 1;
}

int ft_check_plane_component(char **components)
{
    if (ft_count_components(components) != 4)
        return 0;
    if (!ft_check_elements(components[1]))
        return 0;
    if (!ft_check_elements(components[2]))
        return 0;
    if (!ft_check_colors(components[3]))
        return 0;
    // printf("plane components are valid\n");
    return 1;
}

int ft_check_cylinder_component(char **components)
{
    if (ft_count_components(components) != 6)
        return 0;
    if (!ft_check_elements(components[1]))
        return 0;
    if (!ft_check_elements(components[2]))
        return 0;
    if (!ft_check_non_negative(components[3]) || !ft_check_non_negative(components[4]))
        return 0;
    if (!ft_check_colors(components[5]))
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
        return (ft_check_sphere_component(components));
    if (identifier_id == 5)
        return (ft_check_plane_component(components));
    if (identifier_id == 6)
        return (ft_check_cylinder_component(components));
    return 1;
}
                                                                              
int is_identifier(char *identifier)
{   
    // printf("identifier %s\n", identifier);
    
    if (identifier && !ft_strcmp(identifier, "A"))
        return 1;
    if (identifier && !ft_strcmp(identifier, "C"))
       return 2;
    if (identifier && !ft_strcmp(identifier, "L"))
        return 3;
    if (identifier && !ft_strcmp(identifier, "sp"))
        return 4;
    if (identifier && !ft_strcmp(identifier, "pl"))
        return 5;
    if (identifier && !ft_strcmp(identifier, "cy"))
        return 6;
    return 0;
}

//!norminnete
int ft_check_map_components(t_map **map)
{
    map_line    *tmp;
    int         identifier_id;

    int i;
    i = 0;
    tmp = (*map)->lines;
    int count = 0;
    map_line *line =(*map)->lines;
    while (line)
    {
        count++;
        line = line->next;
    }
    while (tmp)
    {        
        identifier_id = is_identifier(tmp->line_component[0]);
        if (!identifier_id)
        {
            printf("line %d: \n", i);
            printf("identifier is not a valid identifier (%s)\n", tmp->line_component[0]);
            return (0);
        }
        if (!ft_check_components(identifier_id, tmp->line_component, ((*map)->scen_elements)))
        {
            printf("Error at map checking line component \n");
            return (0);
        }
        tmp = tmp->next;
        i++;
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
        printf("======================================\n");
        for (int i = 0; map->line_component[i]; i++)
            printf("component %d : %s\n", i, map->line_component[i]);
        // printf("sdfs\n");
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
        if (line && !is_empty_line(line))
            if (!ft_add_line(&map, line))
                return (free(line), close(fd),NULL);
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return (map);
}

/* =====================   map Components generation   ==========================  */

int ft_gen_colors(t_color *color,char *components)
{
    char    **tmp_colors;
 
    tmp_colors = split(components, ",");
    if (!tmp_colors) 
        return 0;
    (color)->r= clamp (ft_atod(tmp_colors[0])  / 255.0f,0,1);
    (color)->g= clamp(ft_atod(tmp_colors[1])  / 255.0f,0,1);
    (color)->b= clamp(ft_atod(tmp_colors[2])  / 255.0f,0,1);
    ft_free_line_components(tmp_colors);
    return (1); 
}

int ft_gen_elements(t_vector *coordinates, char *components)
{
    char **tmp_cord;

    tmp_cord = split(components, ",");
    if (!tmp_cord)
        return (0);
    *coordinates = ft_new_vector(ft_atod(tmp_cord[0]),ft_atod(tmp_cord[1]), ft_atod(tmp_cord[2]));
    ft_free_line_components(tmp_cord);
    return (1);
}

int ft_add_ambient(t_scene **scene, char **components)
{
    t_ambient  *tmp;

    // tmp = (*scene)->ambient;
    tmp = malloc(sizeof(t_ambient));
    tmp->ambient_ration = ft_atod(components[1]);
    ft_gen_colors(&tmp->ambient_color, components[2]);
    (*scene)->ambient = tmp;
    return (1);
}

int ft_add_camera(t_scene **scene, char **components)
{
    s_camera *camera;
    t_point position;
    t_vector dir;
    float     fov;
    
    ft_gen_elements(&position, components[1]);
    ft_gen_elements(&dir, components[2]);
    fov = ft_atod(components[3]);
    camera = malloc(sizeof(s_camera));
    if (!camera)
        return 0;
    camera->fov  = fov;
    camera->w_size = 1500;
    camera->h_size = 700;
    camera->direction = dir;
    camera->origin = position;
    ft_set_camera(&camera);
    (*scene)->camera = camera;
    return 1;
}

int ft_add_light(t_scene **scene, char **components)
{
    t_light *light;

    // light = (*scene)->light;
    light = malloc(sizeof(t_light));
    ft_gen_elements(&light->light_coordinate, components[1]);
    light->light_ration = ft_atod(components[2]);
    ft_gen_colors(&light->light_color, components[3]);
    light->light_coordinate.w = 1;
    (*scene)->light = light;
    return 1;
}
  
t_sphere *ft_new_sphere(char **components)
{
    t_sphere *sphere;
    
    sphere = malloc(sizeof(t_sphere));
    if (!sphere)
        return NULL;
    ft_gen_elements(&sphere->sphere_coordinates, components[1]);
    sphere->sphere_diameter = ft_atod(components[2]);
    ft_gen_colors(&sphere->sphere_color, components[3]);
    sphere->transform = identity_matrix(4);
    sphere->material = default_material();
    ft_gen_colors(&sphere->material->color, components[3]);
    sphere->next = NULL;
    return sphere;
}

int ft_add_sphere(t_scene **scene, char **components)
{
    t_sphere *sphere;
    t_sphere *tmp;
    
    sphere = ft_new_sphere(components);
    if (!sphere)
            return 0;
    tmp = (*scene)->sphere;
    if (!tmp) 
        (*scene)->sphere = sphere;
    else
    {
        while (tmp->next)
            tmp =tmp->next;
        tmp->next = sphere;
    }
    return 1;
}

t_plane *ft_new_plane(char **components)
{
    t_plane *plane;

    plane = malloc(sizeof(t_plane));
    if (!plane)
        return NULL;
    ft_gen_elements(&plane->plane_cordinates, components[1]);
    ft_gen_elements(&plane->plane_normal, components[2]);
    ft_gen_colors(&plane->plane_color, components[3]);
    plane->transform = identity_matrix(4);
    plane->material = default_material();
    plane->material->color = plane->plane_color;
    plane->next = NULL;
    return plane;
}

int ft_add_plane(t_scene **scene, char **components)
{
    t_plane *new;
    t_plane *tmp;
    
    new = ft_new_plane(components);
    tmp = (*scene)->plane;
    if (!tmp)
        (*scene)->plane = new;
    else
    {
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
    return 1;
}

t_cylinder *ft_new_cylinder(char **components)
{
    t_cylinder *new;
    
    new = malloc(sizeof(t_cylinder));
    if (!new)
        return (NULL);
    ft_gen_elements(&new->coordinates, components[1]);
    ft_gen_elements(&new->orientation, components[2]);
    new->raduis = ft_atod(components[3]);
    new->height = ft_atod(components[4]);
    ft_gen_colors(&new->cylinder_color, components[5]);
    new->transform = identity_matrix(4);
    new->transform[0][3] = new->coordinates.x;
    new->transform[1][3] = new->coordinates.y;
    new->transform[2][3] = new->coordinates.z;
    new->material = default_material();
    new->material->color = new->cylinder_color;
    new->next = NULL;
    return new;
}

int ft_add_cylinder(t_scene **scene, char **components)
{
    t_cylinder *new;
    t_cylinder *tmp;

    new = ft_new_cylinder(components);
    tmp = (*scene)->cylinder;
    if (!tmp)
        (*scene)->cylinder = new;
    else
    {
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
    return 1;
}

int ft_add_component(t_scene **scene, int identifier, char **components)
{
    if (identifier == 1)
        return (ft_add_ambient(scene, components));
    if (identifier == 2)
        return (ft_add_camera(scene, components));
    if (identifier == 3)
        return (ft_add_light(scene, components));
    if (identifier == 4)
        return (ft_add_sphere(scene, components));
    if (identifier == 5)
        return (ft_add_plane(scene, components));
    if (identifier == 6)
        return (ft_add_cylinder(scene, components));
    return (1);
}
 
t_scene *ft_generate_scene(map_line *compoenent)
{
    t_scene *new;
    int     identifier_id;

    new = malloc(sizeof(t_scene));
    if (!new)
        return (NULL);
    new->ambient = NULL;
    new->camera = NULL;
    new->light = NULL;
    new->sphere = NULL;
    new->plane = NULL;
    new->cylinder = NULL;
    new->data = NULL;
    while (compoenent)
    {
        identifier_id = is_identifier(compoenent->line_component[0]);
        if (identifier_id)
            ft_add_component(&new, identifier_id, compoenent->line_component);
        compoenent = compoenent->next;
    }
    return (new);
}