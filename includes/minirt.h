/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:25:24 by aghergho          #+#    #+#             */
/*   Updated: 2024/10/19 17:24:29 by aghergho         ###   ########.fr       */
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
    double          sphere_cordinates[3];
    double          sphere_diameter;
    double          sphere_color[3];
    struct sphere   *next;
}   t_sphere;


typedef struct plane
{
    double     plane_cordinates[3];
    double     plane_normal[3];
    double     plane_color[3];
    struct plane   *next;
} t_plane;

typedef struct cylinder
{
    double     cylinder_cordinates[3];
    double     cylinder_normal[3];
    double     cylinder_color[3];
    double     cylinder_diameter;
    double     cylinder_height;
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
    double     *ambient_ration;
    double     ambient_color[3];
} t_ambient;

typedef struct light
{
    double     light_coordinate[3];
    double     light_ration;
    double     light_color[3];
} t_light;

typedef struct g_scene
{
    t_camera    *camera;
    t_ambient   *ambient;
    t_light     *light;
    t_sphere    *sphere;
    t_plane     *plane;
    t_cylinder  *cylinder;
}   scene;

map_line    *ft_gen_sen_map(char *file_name);
void        var_dump_lines(map_line *map);
int ft_check_map_components(t_map **map);
// int         ft_strlen(char *str);
#endif