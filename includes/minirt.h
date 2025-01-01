/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:25:24 by aghergho          #+#    #+#             */
/*   Updated: 2025/01/01 17:31:46 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef miniRT_H
# define miniRT_H

# include <math.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../minilibx-linux/mlx.h"
#include "./get_next_line/get_next_line.h"
#include "./libft/libft.h"

// # define    WIN_WIDTH   1280
// # define    WIN_HEIGHT  720
# define    EPSILON     1e-6f
# define    PI          3.14159265359

typedef struct    s_vector
{
    float    x;
    float    y;
    float    z;
    float    w;
}                t_vector;

typedef struct s_color
{
    int r;
    int g;
    int b;
} t_color;


typedef struct s_ray
{
    t_color     *color;
    t_vector    *origin;
    t_vector    *direction;
    t_vector    *r_lab;    
} t_ray;

typedef t_vector Point3;

typedef struct	s_img
{
	void	*img_ptr;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_img;

typedef struct var
{
    void    *mlx;
    void    *win;
    t_img   img;
    int     color;
}   t_var;

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
    double          sphere_diameter;
    t_vector        *sphere_coordinates;
    t_color         *sphere_color;
    struct sphere   *next;
}   t_sphere;


typedef struct plane
{
    t_vector        *plane_cordinates;
    t_vector        *plane_normal;
    t_color          *plane_color;
    struct plane    *next;
} t_plane;

typedef struct cylinder
{
    t_vector        *coordinates;
    t_vector        *orientation;
    t_color         *cylinder_color;
    double          raduis;
    double          height;
    struct cylinder *next;
} t_cylinder;

typedef struct camera
{
    int             camera_fov;
    float           focal_lenght;
    int             hor_size;
    t_vector        *camera_position;
    t_vector        *camera_dir;
    t_vector        *cam_u;
    t_vector        *cam_v;
    t_vector        *projection_center;
    t_vector        *alignement_vector;
} t_camera;

typedef struct ambient
{
    double          ambient_ration;
    t_color         *ambient_color;
} t_ambient;

typedef struct light
{
    t_vector        *light_coordinate;
    double          light_ration;
    t_color         *light_color;
} t_light;

typedef struct objects
{
    int             object_id;
    void            *object;
    struct objects*  next;
} t_object;

typedef struct s_scene
{
    int         image_height;
    int         image_width;
    float       aspect_ratio;
    float       vp_hight;
    float       vp_width;
    t_vector    *l_corner;
    t_vector    *vp_u;
    t_vector    *vp_v;
    t_vector    *vp_center;
    t_vector    *left_corner;
    t_camera    *camera;
    t_ambient   *ambient;
    t_light     *light;
    t_sphere    *sphere;
    t_plane     *plane;
    t_cylinder  *cylinder;
    t_var       *data;
}   t_scene;

map_line    *ft_gen_scen_map(char *file_name);
t_scene     *ft_generate_scene(map_line *compoenent);
void        var_dump_lines(map_line *map);
int         ft_check_map_components(t_map **map);
float       ft_vec_lenght(t_vector *v);
double       vec_lenght(t_vector *v);
float       vector_dot(t_vector *v1, t_vector *v2);
t_vector    *ft_new_vector(float x, float y, float z);
t_vector    *vector_normilze(t_vector *vec);
t_vector    *vector_multiply_scalar(t_vector *v, float scalar);
t_vector    *vector_cross(t_vector *v1, t_vector *v2);
t_vector    *vector_sub(t_vector *v1, t_vector *v2);
t_vector    *vector_add(t_vector *v1, t_vector *v2);
t_vector    *ft_new_vector(float x, float y, float z);
void        var_dump_vector(t_vector *vec);
t_ray       *create_ray(t_vector *origin, t_vector *direction);
int         gen_ray(t_scene *scene,float x, float y, t_ray **cam_ray);

// int check_intersection(t_ray *ray, t_vector *sphere_center, float radius) ;
// int check_intersection(t_ray *cast_ray,t_vector *inter_point ,t_vector *local_normal,t_vector *locol_color);
t_vector    *vector_copy(t_vector *src);

/* colors operations*/
t_color *ft_new_color(float r, float g, float b);
t_color *ft_add_color(t_color *c1, t_color *c2);
t_color *ft_sub_color(t_color *c1, t_color *c2);
t_color *ft_scale_color(t_color *c, float scalar);
t_color *ft_multiply_color(t_color *c1, t_color *c2);
void    negate_vector(t_vector *vector);
int compare_vector(float a, float b);
/* end of color manipulation*/

/* matrix operations*/
float **ft_create_matrix(int n_rows, int n_cols);
int ft_compare_matrix(float **m1, float **m2, int n_col, int n_row);
void ft_transpose_matrix(float ***m, int n_rows, int n_col);
float **get_minor(float **m, int row, int col, int n);
float determinant(float **m, int n);
/* end of  matrix operation*/


#endif