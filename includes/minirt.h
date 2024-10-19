/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:25:24 by aghergho          #+#    #+#             */
/*   Updated: 2024/10/18 16:53:27 by aghergho         ###   ########.fr       */
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
    int scen_elements[6];
} t_map;

typedef struct g_scene
{
    /*======= ambime properities =========*/
    double     *ambient_ration;
    double     ambient_color[3];

    /*======= light properities =========*/
    double     light_coordinate[3];
    double     light_ration;
    double     light_color[3];    

    /*======= camera properities =========*/
    double     camera_position[3];
    double     camera_cordinates[3];
    int        camera_fow;
    
    /*======= sphere properities =========*/
    double     sphere_cordinates[3];
    double     sphere_diameter;
    double     sphere_color[3];

    /*======= plan properities =========*/
    double     plane_cordinates[3];
    double     plane_normal[3];
    double     plane_color[3];

    /*======= cylinder properities =========*/
    double     cylinder_cordinates[3];
    double     cylinder_normal[3];
    double     cylinder_color[3];
    double     cylinder_diameter;
    double     cylinder_height;
}   scene;

map_line    *ft_gen_sen_map(char *file_name);
void        var_dump_lines(map_line *map);
// int         ft_strlen(char *str);
#endif