/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 03:29:22 by mkartit           #+#    #+#             */
/*   Updated: 2025/03/17 03:29:25 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

void	get_spherical_coordinates(t_vector hit_point, t_sphere *sphere,
		float *u, float *v)
{
	t_vector	local_point;
	float		theta;
	float		phi;

	local_point = vector_sub(hit_point, sphere->sphere_coordinates);
	local_point = vector_normilze(local_point);
	theta = atan2(local_point.z, local_point.x);
	phi = acos(local_point.y);
	*u = (theta + M_PI) / (2 * M_PI);
	*v = phi / M_PI;
}

t_color	sample_texture(t_texture *texture, float u, float v)
{
	int		x;
	int		y;
	int		pixel_index;
	int		color;
	t_color	result;

	u = fmod(u, 1.0f);
	v = fmod(v, 1.0f);
	if (u < 0)
		u += 1.0f;
	if (v < 0)
		v += 1.0f;
	x = (int)(u * texture->width);
	y = (int)(v * texture->height);
	x = x % texture->width;
	y = y % texture->height;
	pixel_index = y * texture->size_line + x * (texture->bpp / 8);
	color = *(int *)(texture->img_data + pixel_index);
	result = int_to_color(color);
	return (result);
}

t_color	int_to_color(int color)
{
	t_color	result;

	result.r = (color >> 16) & 0xFF;
	result.g = (color >> 8) & 0xFF;
	result.b = color & 0xFF;
	return (result);
}
