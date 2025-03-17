/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texturing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 03:12:41 by mkartit           #+#    #+#             */
/*   Updated: 2025/03/17 03:12:42 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

static t_color	get_ambient_color(t_lighting *data)
{
	t_color	eff_color;

	eff_color = ft_multiply_color(data->texture_color, data->light->color);
	return (clamp_color(ft_multiply_color_scalar(eff_color,
				data->material->ambient)));
}

static t_color	get_diffuse_color(t_lighting *data, t_vector light_dir)
{
	float		light_dot_normal;
	t_color		diffuse;
	t_color		eff_color;

	light_dot_normal = vector_dot(light_dir, data->comp->normv);
	if (light_dot_normal < EPSILON)
		return (ft_new_color(0, 0, 0));
	eff_color = ft_multiply_color(data->texture_color, data->light->color);
	diffuse = ft_multiply_color_scalar(eff_color,
			data->material->diffuse * light_dot_normal);
	return (diffuse);
}

static t_color	get_specular_color(t_lighting *data, t_vector light_dir)
{
	float		reflect_dot_camera;
	t_color		specular;
	t_vector	reflected;

	reflected = reflect_vector(negate_vector(light_dir), data->comp->normv);
	reflect_dot_camera = vector_dot(reflected, data->comp->camv);
	if (reflect_dot_camera <= EPSILON)
		return (ft_new_color(0, 0, 0));
	specular = ft_multiply_color_scalar(data->light->color,
			data->material->specular * powf(reflect_dot_camera,
				data->material->shininess));
	return (specular);
}

t_color	get_textured_lighting_color(t_lighting *data)
{
	t_vector	light_dir;
	t_color		ambient;
	t_color		diffuse;
	t_color		specular;
	t_color		final_color;

	ambient = get_ambient_color(data);
	if (data->shadow)
		return (ambient);
	light_dir = vector_normilze(vector_sub(data->light->coordinate,
				data->comp->point));
	diffuse = get_diffuse_color(data, light_dir);
	specular = get_specular_color(data, light_dir);
	final_color = clamp_color(ft_add_color(ft_add_color(ambient, diffuse),
				specular));
	return (final_color);
}

t_color	handle_textured_sphere(t_world *world, t_compose *comp, t_light *light,
		t_material *material)
{
	float		u;
	float		v;
	int			shadowed;
	t_color		base_color;
	t_lighting	data;

	u = 0;
	v = 0;
	get_spherical_coordinates(comp->over_point, (t_sphere *)comp->obj, &u, &v);
	base_color = sample_texture(((t_sphere *)comp->obj)->texture, u, v);
	base_color.r /= 255.0f;
	base_color.g /= 255.0f;
	base_color.b /= 255.0f;
	shadowed = is_shadowed(world, light, comp->over_point);
	data = (t_lighting){base_color, material, light, comp, shadowed};
	return (get_textured_lighting_color(&data));
}
