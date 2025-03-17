/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coloring_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:43:04 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 09:08:54 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	is_shadowed(t_world *world, t_light *light, t_point point)
{
	t_vector		v;
	t_vector		direction;
	float			distance;
	t_ray			r;
	t_intersection	intersections;

	v = vector_sub(light->coordinate, point);
	direction = vector_normilze(v);
	r = create_ray(point, direction);
	distance = vec_lenght(v);
	intersections = intersect_world(world, r);
	if (intersections.n_sol > 0 && intersections.t1 >= EPSILON
		&& intersections.t1 < distance)
		return (1);
	return (0);
}

t_material	*get_object_material(void *obj, int obj_type)
{
	if (obj_type == SHAPE_SPHERE)
		return (((t_sphere *)obj)->material);
	else if (obj_type == SHAPE_PLANE)
		return (((t_plane *)obj)->material);
	else if (obj_type == SHAPE_CYLINDER)
		return (((t_cylinder *)obj)->material);
	else if (obj_type == SHAPE_CONE)
		return (((t_cone *)obj)->material);
	return (NULL);
}

t_color	shading_hit(t_world *world, t_compose *comp)
{
	t_color		color;
	t_material	*material;
	t_light		*current_light;

	color = ft_new_color(0, 0, 0);
	material = get_object_material(comp->obj, comp->obj_type);
	current_light = world->light;
	while (current_light)
	{
		color = ft_add_color(color, process_light(world, comp, current_light,
					material));
		current_light = current_light->next;
	}
	return (clamp_color(color));
}

t_color	get_color_at(t_world *world, t_ray ray)
{
	t_intersection	inter;
	t_compose		*comp;
	t_color			res;

	inter = intersect_world(world, ray);
	if (inter.n_sol <= 0)
		return (ft_new_color(0, 0, 0));
	comp = prepare_computations(inter, ray);
	res = shading_hit(world, comp);
	free(comp);
	return (res);
}
