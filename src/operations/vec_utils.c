/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 00:34:13 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 01:25:14 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

inline float	radian(float degree)
{
	return (degree * PI / 180);
}

t_plight	*ft_new_plight(t_color color, t_point point)
{
	t_plight	*light;

	light = (t_plight *)malloc(sizeof(t_plight));
	if (!light)
		return (NULL);
	light->color = ft_new_color(color.r, color.g, color.b);
	light->position = ft_new_point(point.x, point.y, point.z);
	return (light);
}

/*
	Specular=Lightcolor×MaterialSpecular×(cos(θ))
*/

void	get_color_helper(t_lihgt_color *light_col, t_material *material,
		t_compose *comp, t_light *light)
{
	light_col->diffuse = ft_multiply_color_scalar(light_col->eff_color,
			material->diffuse * light_col->light_dot_normal);
	light_col->reflect_dot_camera = vector_dot(reflect_vector(
				negate_vector(light_col->light_dir_normal),
				comp->normv), comp->camv);
	if (light_col->reflect_dot_camera <= EPSILON)
		light_col->specular = ft_new_color(0, 0, 0);
	else
		light_col->specular = ft_multiply_color_scalar(light->color,
				material->specular * powf(light_col->reflect_dot_camera,
					material->shininess));
}

t_color	get_lighting_color(t_material *material, t_light *light,
		t_compose *comp, t_color base_color)
{
	t_lihgt_color	*light_col;
	t_color			res;

	light_col = malloc(sizeof(t_lihgt_color));
	light_col->eff_color = ft_multiply_color(base_color, light->color);
	light_col->ambient = clamp_color(ft_multiply_color_scalar(
				light_col->eff_color, material->ambient));
	res = light_col->ambient;
	if (comp->shadow)
		return (free(light_col), res);
	light_col->light_dir_normal = vector_normilze(vector_sub(light->coordinate,
				comp->point));
	light_col->light_dot_normal = vector_dot(light_col->light_dir_normal,
			comp->normv);
	if (light_col->light_dot_normal < EPSILON)
		return (free(light_col), res);
	else
		get_color_helper(light_col, material, comp, light);
	res = ft_add_color(light_col->specular, light_col->diffuse);
	res = ft_add_color(res, light_col->ambient);
	return (free(light_col), clamp_color(res));
}

t_material	*default_material(void)
{
	t_material	*m;

	m = malloc(sizeof(t_material));
	if (!m)
		return (NULL);
	m->color = ft_new_color(1.0, 1.0, 1.0);
	m->ambient = 0.1;
	m->diffuse = 0.9;
	m->specular = 0.9;
	m->shininess = 200.0;
	return (m);
}
