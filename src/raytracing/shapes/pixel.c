/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pixel.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:50:35 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 10:24:41 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../../includes/minirt.h"

void	my_pixel_put(t_img *img, int x, int y, int color)
{
	int	offset;

	offset = (img->line_length * y) + (x * (img->bits_per_pixel / 8));
	*((unsigned int *)(img->addr + offset)) = color;
}

t_ray	transform(t_ray ray, float **m)
{
	t_vector	dir;
	t_point		origin;

	dir = ft_multiply_matrix_vec(m, ray.direction);
	origin = ft_multiply_matrix_vec(m, ray.origin);
	return (create_ray(origin, dir));
}

int	calculate_pixel_color(t_world *world, t_ray ray)
{
	t_color	color;
	int		pixel_color;

	color = get_color_at(world, ray);
	pixel_color = (255 << 24) | (int)clamp((float)(255.999 * color.r), 0,
			255) << 16 | (int)clamp((float)(255.999 * color.g), 0,
			255) << 8 | (int)clamp((float)(255.999 * color.b), 0, 255);
	return (pixel_color);
}

void	render_pixel(t_scene *scene, t_scamera *cam, int x,
		int y)
{
	t_ray	ray;
	int		pixel_color;

	ray = get_ray_pixel(cam, x, y, 0.5);
	pixel_color = calculate_pixel_color(scene->world, ray);
	my_pixel_put(&scene->data->img, x, y, pixel_color);
}
