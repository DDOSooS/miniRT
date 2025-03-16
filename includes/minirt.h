/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:25:24 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/16 14:01:18 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../minilibx-linux/mlx.h"
# include "./get_next_line/get_next_line.h"
# include "./libft/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

// # define SCREEN_WIDTH 1280.f
// # define SCREEN_HEIGHT 720.f

#define SCREEN_WIDTH 80.f
#define SCREEN_HEIGHT 720.f

# define EPSILON 1e-4f
# define PI 3.14159265359
# define ESC_KEY 65307

typedef struct s_vector
{
	float			x;
	float			y;
	float			z;
	float			w;
}					t_vector;

typedef struct s_color
{
	float			r;
	float			g;
	float			b;
}					t_color;

typedef enum e_shape_type
{
	SHAPE_SPHERE,
	SHAPE_PLANE,
	SHAPE_CYLINDER,
	SHAPE_CONE
}					t_shape_type;

typedef struct s_ray
{
	t_vector		origin;
	t_vector		direction;
}					t_ray;

typedef struct s_intersection
{
	int				n_sol;
	float			t1;
	void			*object;
	int				type;
}					t_intersection;

typedef t_vector	t_point;

typedef struct s_img
{
	void			*img_ptr;
	char			*addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
}					t_img;

typedef struct s_material
{
	t_color			color;
	float			ambient;
	float			diffuse;
	float			specular;
	float			shininess;
}					t_material;

typedef struct light_s
{
	t_color			color;
	t_point			position;
}					t_plight;

typedef struct var
{
	void			*mlx;
	void			*win;
	t_img			img;
	int				color;
}					t_var;

typedef struct t_scamera
{
	float			h_size;
	float			w_size;
	float			half_h_size;
	float			half_w_size;
	float			fov;
	float			**transform;
	float			pixel_size;
	t_point			origin;
	t_vector		direction;
}					t_scamera;

typedef struct lines
{
	char			**line_component;
	struct lines	*next;
}					t_map_line;

typedef struct map
{
	t_map_line		*lines;
	int				scen_elements[3];
}					t_map;

typedef struct s_texture
{
	t_color			*colors;
	char			*img_data;
	void			*img_ptr;
	int				width;
	int				height;
	int				size_line;
	int				bpp;
	int				endian;
}					t_texture;

typedef struct sphere
{
	float			sphere_diameter;
	t_vector		sphere_coordinates;
	t_color			sphere_color;
	t_color			checkered_color;
	float			**transform;
	t_material		*material;
	t_texture		*texture;
	int				has_texture;
	int				has_checkered;
	struct sphere	*next;
}					t_sphere;

typedef struct plane
{
	t_vector		plane_cordinates;
	t_vector		plane_normal;
	t_color			plane_color;
	t_material		*material;
	float			**transform;
	struct plane	*next;
}					t_plane;

typedef struct cylinder
{
	t_point			coordinates;
	t_vector		orientation;
	t_color			cylinder_color;
	double			raduis;
	double			height;
	float			**transform;
	t_material		*material;
	struct cylinder	*next;
}					t_cylinder;

typedef struct cone
{
	t_vector		apex;
	t_vector		axis;
	float			height;
	float			radius;
	t_color			color;
	t_material		*material;
	float			**transform;
	struct cone		*next;
}					t_cone;

typedef struct camera
{
	int				camera_fov;
	float			focal_lenght;
	int				hor_size;
	t_vector		camera_position;
	t_vector		camera_dir;
	t_vector		cam_u;
	t_vector		cam_v;
	t_vector		projection_center;
	t_vector		alignement_vector;
}					t_camera;

typedef struct ambient
{
	double			ambient_ration;
	t_color			ambient_color;
}					t_ambient;

typedef struct light
{
	t_point			coordinate;
	double			ration;
	t_color			color;
	struct light	*next;
}					t_light;

typedef struct u_objects
{
	int					object_id;
	void				*object;
	struct u_objects	*next;
}					t_object;

typedef struct s_compose
{
	t_intersection	intersection;
	void			*obj;
	int				obj_type;
	float			t;
	t_vector		camv;
	t_vector		normv;
	t_point			point;
	int				inside;
	t_point			over_point;
}					t_compose;

typedef struct s_shape
{
	t_shape_type	type;
	union
	{
		t_sphere	*sphere;
		t_plane		*plane;
		t_cylinder	*cylinder;
		t_cone		*cone;
	} u_objects;
	struct s_shape	*next;
}					t_shape;

typedef struct s_world
{
	int				n_objects;
	t_shape			*shape;
	t_light			*light;
}					t_world;

typedef struct s_scene
{
	t_scamera		*camera;
	t_ambient		*ambient;
	t_light			*light;
	t_sphere		*sphere;
	t_plane			*plane;
	t_cylinder		*cylinder;
	t_cone			*cone;
	t_var			*data;
	t_map			*map;
	t_world			*world;
}					t_scene;

int					ft_close_window(t_scene *scen);
void				my_pixel_put(t_img *img, int x, int y, int color);
t_map_line			*ft_gen_scen_map(char *file_name);
int					render_image(t_scene *scene, t_world *t_world,
						t_scamera *cam);
int					ft_generate_scene(t_map_line *compoenent, t_scene **scene);
void				var_dump_lines(t_map_line *map);
int					ft_check_map_components(t_map **map);
float				ft_vec_lenght(t_vector *v);
double				vec_lenght(t_vector v);
float				vector_dot(t_vector v1, t_vector v2);
t_vector			ft_new_vector(float x, float y, float z);
t_vector			vector_normilze(t_vector vec);
t_vector			vector_multiply_scalar(t_vector v, float scalar);
t_vector			vector_cross(t_vector v1, t_vector v2);
t_vector			vector_sub(t_vector v1, t_vector v2);
t_vector			vector_add(t_vector v1, t_vector v2);
void				var_dump_vector(t_vector vec);
void				init_scene(t_scene *scene);
t_ray				create_ray(t_vector origin, t_vector direction);
int					gen_ray(t_scene *scene, float x, float y, t_ray cam_r);
void				ft_set_camera(t_scamera **camera);
t_vector			vector_copy(t_vector src);
t_sphere			*ft_new_sphere(char **components, t_scene **scene);

/* colors operations*/
t_color				ft_new_color(float r, float g, float b);
t_color				ft_add_color(t_color c1, t_color c2);
t_color				ft_sub_color(t_color c1, t_color c2);
t_color				ft_scale_color(t_color c, float scalar);
t_color				ft_multiply_color(t_color c1, t_color c2);
t_vector			negate_vector(t_vector vector);
int					compare_vector(float a, float b);
t_vector			normalize_at_plane_pos(t_plane *plane, t_point w_p);
t_vector			reflect_vector(t_vector, t_vector);
t_color				ft_multiply_color_scalar(t_color color, float scalar);
/* end of color manipulation*/

/* matrix operations*/
float				**ft_create_matrix(int n_rows, int n_cols);
float				**ft_multiply_matrix(float **m1, float **m2, int n_cols,
						int n_row);
int					ft_compare_matrix(float **m1, float **m2, int n_col,
						int n_row);
t_vector			ft_multiply_matrix_vec(float **m, t_vector v);
float				**identity_matrix(int n);
void				ft_transpose_matrix(float ***m, int n_rows, int n_col);
float				**get_minor(float **m, int row, int col, int n);
float				determinant(float **m, int n);
float				**inverse_matrix(float **m, int n);
void				ft_scale_matrix(float ***m, float scale, int n);
float				**ft_scaling_matrix(float x, float y, float z, int inverse);
float				**ft_translate_matrix(t_point point, int inverse);
t_point				ft_transform(t_point p1, t_point p2, int inverse);
float				**shearing_matrix(int *coord);
t_point				ft_new_point(float x, float y, float z);
void				print_matrix(float **matrix, int rows, int cols);
t_point				ft_scale(t_point scale, t_point point, int inverse);

float				**rotate_x(float rad);
float				**rotate_y(float rad);
float				**rotate_z(float rad);
void				print_matrix(float **matrix, int rows, int cols);
float				**shearing_matrix(int *coord);

void				ft_free_matrix(float **m, int n);
t_material			*default_material(void);
t_plight			*ft_new_plight(t_color color, t_point point);
/* end of  matrix operation*/

/* ray manipulation*/
t_point				position(t_ray ray, float distance);
t_ray				create_ray(t_point origin, t_vector dir);
t_intersection		ft_intersect_sphere(t_ray ray, t_sphere *sphere);
t_intersection		ft_new_intersection(float t, void *object, int type);
t_intersection		ray_hit(t_intersection *inters, int count);
t_ray				transform(t_ray ray, float **m);
t_ray				transform(t_ray ray, float **m);
t_point				position(t_ray ray, float distance);
t_color				get_lighting_color(t_material *material, t_light *light,
						t_compose *comp, int shadow, t_color base_color);
t_vector			normilize_at_sphere_pos(t_sphere *sphere, t_point w_p);
/* end of ray manipulation functions*/

t_world				*default_world(t_scene *scene);
t_intersection		intersect_world(t_world *world, t_ray ray);
t_sphere			*default_sphere(void);
t_compose			*prepare_computations(t_intersection inter, t_ray ray);
t_color				shading_hit(t_world *world, t_compose *comp);
t_color				get_color_at(t_world *world, t_ray ray);
float				**get_view_transform(t_point from_v, t_point to_v,
						t_vector up_v);
t_ray				get_ray_pixel(t_scamera *cam, float x, float y, float edge);
t_color				clamp_color(t_color color);

int					is_shadowed(t_world *world, t_light *light, t_point point);
float				clamp(float value, float min, float max);
t_vector			ft_scale_vector(t_vector vector, float scale);
t_point				ft_multiply_matrix_point(float **m, t_point v);
float				**create_rotation_matrix_from_vector(t_vector orientation);
t_point				ft_scale_point(t_point vector, float scale);

/* destroy scen functions */
void				ft_destroy_scene(t_scene *scen);
void				ft_free_map(t_map_line **map_lines);
void				ft_free_map_line(t_map *map);
#endif /* MINIRT_H */