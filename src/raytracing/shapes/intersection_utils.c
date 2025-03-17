/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:50:09 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 10:00:43 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../../includes/minirt.h"

t_intersection	ray_hit(t_intersection *inters, int count)
{
	float			min_t;
	t_intersection	result;
	t_intersection	*tmp;
	int				i;

	min_t = (float)INT_MAX;
	tmp = &inters[0];
	i = 0;
	while (i < count)
	{
		if (inters[i].t1 > 0 && inters[i].t1 < min_t)
		{
			min_t = inters[i].t1;
			tmp = &inters[i];
		}
		i++;
	}
	result.n_sol = tmp->n_sol;
	result.t1 = tmp->t1;
	result.object = tmp->object;
	result.type = tmp->type;
	return (result);
}

t_intersection	ft_new_intersection(float t, void *object, int type)
{
	t_intersection	intersection;

	intersection.t1 = t;
	intersection.object = object;
	intersection.type = type;
	return (intersection);
}

void	intersect_world_checker(t_shape *current, t_ray ray,
		t_intersection *inter, int *i)
{
	if (current->type == SHAPE_SPHERE)
		inter[++(*i)] = ft_intersect_sphere(ray, current->u_objects.sphere);
	else if (current->type == SHAPE_PLANE)
		inter[++(*i)] = ft_intersect_plane(ray, current->u_objects.plane);
	else if (current->type == SHAPE_CYLINDER)
		inter[++(*i)] = ft_intersect_cylinder(ray, current->u_objects.cylinder);
	else if (current->type == SHAPE_CONE)
		inter[++(*i)] = ft_intersect_cone(ray, current->u_objects.cone);
}

t_intersection	intersect_world(t_world *world, t_ray ray)
{
	t_intersection	res;
	t_intersection	*inter;
	t_shape			*current;
	int				i;

	if (!world->shape)
		return ((t_intersection){0, 0, NULL, 0});
	current = world->shape;
	inter = malloc(sizeof(t_intersection) * world->n_objects);
	i = -1;
	while (current)
	{
		intersect_world_checker(current, ray, inter, &i);
		current = current->next;
	}
	res = ray_hit(inter, world->n_objects);
	return (free(inter), res);
}

t_compose	*prepare_computations(t_intersection inter, t_ray ray)
{
	t_compose	*comp;

	comp = init_composition(inter, ray);
	if (comp->obj_type == SHAPE_SPHERE)
		comp->normv = normilize_at_sphere_pos((t_sphere *)(inter.object),
				comp->point);
	else if (comp->obj_type == SHAPE_PLANE)
		comp->normv = normalize_at_plane_pos((t_plane *)(inter.object));
	else if (comp->obj_type == SHAPE_CYLINDER)
		comp->normv = normalize_at_cylinder_pos((t_cylinder *)(inter.object),
				comp->point);
	else if (comp->obj_type == SHAPE_CONE)
		comp->normv = normalize_at_cone_pos((t_cone *)(inter.object),
				comp->point);
	comp->normv = vector_normilze(comp->normv);
	if (vector_dot(comp->normv, comp->camv) < 0.0)
		comp->normv = negate_vector(comp->normv);
	comp->over_point = point_add(comp->point, ft_scale_point(comp->normv,
				EPSILON));
	return (comp);
}
