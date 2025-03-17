/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 02:10:05 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 08:15:08 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	ft_add_camera(t_scene **scene, char **components)
{
	t_scamera	*camera;
	t_point		position;
	t_vector	dir;
	float		fov;

	ft_gen_elements(&position, components[1]);
	ft_gen_elements(&dir, components[2]);
	fov = ft_atod(components[3]);
	camera = malloc(sizeof(t_scamera));
	if (!camera)
		return (0);
	camera->fov = fov;
	camera->h_size = SCREEN_HEIGHT;
	camera->w_size = SCREEN_WIDTH;
	camera->direction = dir;
	camera->origin = position;
	ft_set_camera(&camera);
	(*scene)->camera = camera;
	return (1);
}

void	ft_set_camera(t_scamera **camera)
{
	float	aspect;
	float	half_view;

	aspect = (*camera)->w_size / (*camera)->h_size;
	half_view = tan((*camera)->fov / 2.0f);
	if (aspect >= 1.0f)
	{
		(*camera)->half_w_size = half_view;
		(*camera)->half_h_size = half_view / aspect;
	}
	else
	{
		(*camera)->half_w_size = half_view * aspect;
		(*camera)->half_h_size = half_view;
	}
	(*camera)->pixel_size = (*camera)->half_w_size * 2.0f / (*camera)->w_size;
	(*camera)->transform = get_view_transform((*camera)->origin,
			vector_normilze((*camera)->direction), ft_new_vector(0, 1, 0));
}

/*
	X-axis corresponds to left/right
	Y-axis corresponds to up/down
	Z-axis corresponds to forward/backward (with negative Z being forward)
*/

/*	camera transform matrix that transform
	from world coordinates to camera system Co
*/

void	set_view_matrix(float **m, t_vector left_v, t_vector up_n,
		t_vector forward_v)
{
	m[0][0] = left_v.x;
	m[0][1] = left_v.y;
	m[0][2] = left_v.z;
	m[1][0] = up_n.x;
	m[1][1] = up_n.y;
	m[1][2] = up_n.z;
	m[2][0] = -forward_v.x;
	m[2][1] = -forward_v.y;
	m[2][2] = -forward_v.z;
}

float	**get_view_transform(t_point from_v, t_vector to_v, t_vector up_v)
{
	t_view_matrix	view_mx;
	float			**view_transform;
	float			**res;
	float			**traslate_mx;

	view_mx.forward_v = vector_normilze(to_v);
	view_mx.up_n = vector_normilze(up_v);
	view_mx.left_v = vector_cross(view_mx.forward_v, view_mx.up_n);
	up_v = vector_cross(view_mx.left_v, view_mx.forward_v);
	view_transform = identity_matrix(4);
	set_view_matrix(view_transform, view_mx.left_v, up_v, view_mx.forward_v);
	traslate_mx = ft_translate_matrix(ft_new_point(-from_v.x, -from_v.y,
				-from_v.z), 1);
	res = ft_multiply_matrix(view_transform, traslate_mx, 4, 4);
	ft_free_matrix(view_transform, 4);
	ft_free_matrix(traslate_mx, 4);
	return (res);
}
