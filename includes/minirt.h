/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:25:24 by aghergho          #+#    #+#             */
/*   Updated: 2024/10/21 10:33:16 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef miniRT_H
# define miniRT_H

# include <math.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./get_next_line/get_next_line.h"
#include "./libft/libft.h"


typedef struct lines
{
    char **line_component;
    struct lines *next;
}   map_line;

typedef struct map
{
    map_line  *lines;
    int scen_elements[3];
} t_map;

typedef struct sphere
{
    double          sphere_coordinates[3];
    double          sphere_diameter;
    int          sphere_color[3];
    struct sphere   *next;
}   t_sphere;


typedef struct plane
{
    double     plane_cordinates[3];
    double     plane_normal[3];
    int     plane_color[3];
    struct plane   *next;
} t_plane;

typedef struct cylinder
{
    double     coordinates[3];
    double     orientation[3];
    int     colors[3];
    double     diameter;
    double     height;
    struct cylinder   *next;
} t_cylinder;

typedef struct camera
{
    double     camera_position[3];
    double     camera_cordinates[3];
    int        camera_fow;
} t_camera;

typedef struct ambient
{
    double     ambient_ration;
    int     ambient_color[3];
} t_ambient;

typedef struct light
{
    double     light_coordinate[3];
    double     light_ration;
    int         light_color[3];
} t_light;

typedef struct g_scene
{
    t_camera    *camera;
    t_ambient   *ambient;
    t_light     *light;
    t_sphere    *sphere;
    t_plane     *plane;
    t_cylinder  *cylinder;
}   t_scene;

map_line    *ft_gen_scen_map(char *file_name);
void        var_dump_lines(map_line *map);
t_scene *ft_generate_scene(map_line *compoenent);
int ft_check_map_components(t_map **map);
// int         ft_strlen(char *str);
#endif