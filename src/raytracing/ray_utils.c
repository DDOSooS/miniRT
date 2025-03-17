/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:52:52 by mkartit           #+#    #+#             */
/*   Updated: 2025/03/17 10:23:27 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_ray	create_ray(t_point origin, t_vector dir)
{
	t_ray	new;

	new.origin = origin;
	new.direction = dir;
	return (new);
}

void	rotation_matrix_helper(t_matrix tr_mx, t_vector axis,
		float rotation_matrix[4][4])
{
	rotation_matrix[1][1] = tr_mx.cos_theta + axis.y * axis.y
		* tr_mx.one_minus_cos;
	rotation_matrix[1][2] = axis.y * axis.z * tr_mx.one_minus_cos - axis.x
		* tr_mx.sin_theta;
	rotation_matrix[1][3] = 0.0f;
	rotation_matrix[2][0] = axis.z * axis.x * tr_mx.one_minus_cos - axis.y
		* tr_mx.sin_theta;
	rotation_matrix[2][1] = axis.z * axis.y * tr_mx.one_minus_cos + axis.x
		* tr_mx.sin_theta;
	rotation_matrix[2][2] = tr_mx.cos_theta + axis.z * axis.z
		* tr_mx.one_minus_cos;
	rotation_matrix[2][3] = 0.0f;
	rotation_matrix[3][0] = 0.0f;
	rotation_matrix[3][1] = 0.0f;
	rotation_matrix[3][2] = 0.0f;
	rotation_matrix[3][3] = 1.0f;
}

void	create_rotation_matrix(t_vector axis, float angle,
		float rotation_matrix[4][4])
{
	t_matrix	tr_mx;
	float		length;

	tr_mx.cos_theta = cos(angle);
	tr_mx.sin_theta = sin(angle);
	tr_mx.one_minus_cos = 1.0f - tr_mx.cos_theta;
	length = sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
	axis.x /= length;
	axis.y /= length;
	axis.z /= length;
	rotation_matrix[0][0] = tr_mx.cos_theta + axis.x * axis.x
		* tr_mx.one_minus_cos;
	rotation_matrix[0][1] = axis.x * axis.y * tr_mx.one_minus_cos - axis.z
		* tr_mx.sin_theta;
	rotation_matrix[0][2] = axis.x * axis.z * tr_mx.one_minus_cos + axis.y
		* tr_mx.sin_theta;
	rotation_matrix[0][3] = 0.0f;
	rotation_matrix[1][0] = axis.y * axis.x * tr_mx.one_minus_cos + axis.z
		* tr_mx.sin_theta;
	rotation_matrix_helper(tr_mx, axis, rotation_matrix);
}

/*
linear algebraic transformation
(M^-1)^T * N
*/
t_ray	get_ray_pixel(t_scamera *cam, float x, float y, float edge)
{
	t_ray		ray;
	t_point		pixel_world;
	t_point		pixel;
	float		**inv;
	t_world_sys	system;

	system.world_x = cam->half_w_size - (x + edge + EPSILON) * cam->pixel_size;
	system.world_y = cam->half_h_size - (y + edge + EPSILON) * cam->pixel_size;
	inv = inverse_matrix(cam->transform, 4);
	if (!inv)
		inv = cam->transform;
	pixel = ft_new_point(-system.world_x, -system.world_y, -1);
	pixel_world = ft_multiply_matrix_vec(inv, pixel);
	ray.origin = ft_multiply_matrix_vec(inv, ft_new_point(0, 0, 0));
	ray.direction = vector_normilze(vector_sub(pixel_world, ray.origin));
	if (inv != cam->transform)
		ft_free_matrix(inv, 4);
	return (ray);
}

int	render_image(t_scene *scene, t_scamera *cam)
{
	int		total_pixels;
	int		current_pixel;

	float (x), (y);
	total_pixels = cam->w_size * cam->h_size;
	current_pixel = 0;
	y = -1;
	mlx_clear_window(scene->data->mlx, scene->data->win);
	while (++y < cam->h_size)
	{
		x = -1;
		while (++x < cam->w_size)
		{
			render_pixel(scene, cam, x, y);
			current_pixel++;
		}
		display_progress(scene, cam, current_pixel, total_pixels);
	}
	mlx_clear_window(scene->data->mlx, scene->data->win);
	mlx_put_image_to_window(scene->data->mlx, scene->data->win,
		scene->data->img.img_ptr, 0, 0);
	setup_window_hooks(scene);
	return (0);
}
